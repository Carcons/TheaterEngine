#include <fstream>

#include <filesystem>
namespace stdfs = std::filesystem;

#include <algorithm>

#include <json.hpp>
using json = nlohmann::json;

#include <Theater/Engine/Actor/Actor.hpp>
#include <Theater/Engine/Graphics/Shader.hpp>
#include <Theater/Engine/Graphics/RenderingSystem.hpp>
#include <Theater/Engine/Platform/WindowSystem.hpp>
#include <Theater/Engine/Platform/Joystick.h>
#include <Theater/Engine/Game/ActorRegistry.hpp>
#include <Theater/Engine/Game/Game.hpp>


namespace
{
    json& GetJson(void* pJson)
    {
        return *reinterpret_cast<json*>(pJson);
    }
}

namespace Theater
{
    namespace Detail
    {
        void GameInitializer(const GameplayInitData* pData)
        {
            Game::Get().OnGameReady_External();
        }

        void* GetGameData()
        {
            return Game::GetPtr()->m_pGameData;
        }

        void WriteGameData()
        {
            Game::Get().SerializeGameData();
        }

        void GameOnUpdate(float dt)
        {
            Game::Get().OnUpdate(dt);
        }

        void GameOnDraw()
        {
            Game::Get().OnDraw();
        }
    }
}

namespace Theater
{
    Game::Game() 
    {
        m_pJoystick = nullptr;
        m_pGameData = new json;
        DeserializeGameData();
    }

    Game::~Game()
    {

        for (auto* pActor : m_PendingActors)
            delete pActor;
        for (auto* pActor : m_PendingRemove)
            delete pActor;
        for (auto* pActor : m_Actors)
            delete pActor;

        if (m_pJoystick)
            DeleteJoystick(m_pJoystick);


        delete m_pGameData;
        m_pGameData = nullptr;
    }

    void Game::SetVSync(bool enable)
    {
        
    }

    void Game::OnUpdate(float deltaTime)
    {
        if (m_pJoystick)
            m_pJoystick->UpdateState();

        if (!m_PendingRemove.empty()) 
        {
            const auto It = std::remove_if(
                m_Actors.begin(),
                m_Actors.end(),
                [&](Actor* pActor) 
                {
                    return m_PendingRemove.find(pActor) != m_PendingRemove.end();
                }
            );
            m_Actors.erase(It, m_Actors.end());

            for (Actor* pActor : m_PendingRemove)
            {
                delete pActor;
            }
            m_PendingRemove.clear();
        }

        if (!m_PendingActors.empty()) 
        {
            m_Actors.insert(m_Actors.end(), m_PendingActors.begin(), m_PendingActors.end());
            m_PendingActors.clear();
        }

        m_Iterating = true;

        for (auto* pActor : m_Actors) 
        {
            if (pActor->m_Alive) {
                pActor->OnUpdate(deltaTime);
            }
            else
            {
                m_PendingRemove.insert(pActor);
            }
        }

        m_Iterating = false;
    }

    void Game::OnGameReady_External()
    {
        m_pJoystick = NewJoystick();
        OnGameReady();
        for (Actor* pActor : m_Actors)
        {
            pActor->OnGameReady();
        }
    }

    void Game::OnDraw()
    {
        m_Iterating = true;

        RenderingSystem::Get().SetDepth();
        RenderingSystem::Get().ClearAllBuffers();

        RenderPass();

        m_Iterating = false;
    }

    void Game::AddActor(Actor* pActor)
    {
        if (!pActor)
            return;

        if (m_Iterating)
        {
            m_PendingActors.push_back(pActor);
            return;
        }

        m_Actors.push_back(pActor);
    }

    void Game::RemoveActor(uuid UUID)
    {
        if (m_Iterating)
        {
            m_PendingRemove.insert(GetActorByUUID(UUID));
            return;
        }

        const auto It = std::remove_if(m_Actors.begin(), m_Actors.end(), 
            [&](Actor* pActor)
            { 
                if (pActor->m_UUID == UUID)
                {
                    delete pActor;
                    pActor = nullptr;
                    return true;
                }
                return false;
            }
        );

        if (It != m_Actors.end())
            m_Actors.erase(It, m_Actors.end());
    }

    void Game::SetProjectionMat(const Matrix4& projMat)
    {
        m_ProjMatrix = projMat;
        RenderingSystem::Get().GetMeshShader()->SetUniform("_uEngineProjMat", m_ProjMatrix);
    }

    void Game::SetViewMat(const Matrix4& viewMat)
    {
        m_ViewMatrix = viewMat;
        RenderingSystem::Get().GetMeshShader()->SetUniform("_uEngineViewMat", m_ViewMatrix);
    }

    Actor* Game::GetActorByUUID(uuid UUID)
    {
        for (auto* pActor : m_Actors)
        {
            if (pActor->m_UUID == UUID)
                return pActor;
        }
        return nullptr;
    }

    void Game::RenderPass()
    {
        Shader& shader = *RenderingSystem::Get().GetMeshShader();
        shader.Bind();

        for (auto* pActor : m_Actors)
        {
            if (pActor && pActor->m_Alive)
            {
                pActor->OnDraw(shader);
            }
        }

        shader.UnBind();
    }

    void Game::SerializeGameData()
    {
        std::ofstream file(Detail::GAME_DATA, std::ios::binary);

        if (!GetJson(m_pGameData).empty())
        {
            std::vector<uint8_t> binaryData = nlohmann::json::to_cbor(GetJson(m_pGameData));
            file.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
        }
        else
        {

        }
        file.close();
    }

    void Game::DeserializeGameData()
    {
        using namespace Theater::Detail;

        json& jsonObject = GetJson(m_pGameData);

        if (stdfs::exists(Detail::GAME_DATA))
        {
            std::ifstream fileReader(Detail::GAME_DATA, std::ios::binary);
            if (fileReader.peek() != std::ifstream::traits_type::eof())
            {
                std::vector<uint8_t> binaryData((std::istreambuf_iterator<char>(fileReader)), std::istreambuf_iterator<char>());
                jsonObject = json::from_cbor(binaryData);
            }

            fileReader.close();
        }
        else
        {
            std::ofstream file(Detail::GAME_DATA, std::ios::binary);
            file.close();

            return;
        }

        // Load Actors Instances from Json
        for (auto& entry : jsonObject)
        {
            if (entry.contains("Actor"))
            {
                const uuid ClassUUID = entry["Actor"]["ClassUUID"];
                if (Actor* const pActor = ActorRegistry::Get()->CreateInstance(ClassUUID))
                {
                    pActor->m_UUID = entry["Actor"]["UUID"];

                    pActor->m_Transform.Position.x = entry["Actor"]["Transform"]["Position"]["x"];
                    pActor->m_Transform.Position.y = entry["Actor"]["Transform"]["Position"]["y"];
                    pActor->m_Transform.Position.z = entry["Actor"]["Transform"]["Position"]["z"];

                    pActor->m_Transform.Rotation.x = entry["Actor"]["Transform"]["Rotation"]["x"];
                    pActor->m_Transform.Rotation.y = entry["Actor"]["Transform"]["Rotation"]["y"];
                    pActor->m_Transform.Rotation.z = entry["Actor"]["Transform"]["Rotation"]["z"];

                    pActor->m_Transform.Scale.x = entry["Actor"]["Transform"]["Scale"]["x"];
                    pActor->m_Transform.Scale.y = entry["Actor"]["Transform"]["Scale"]["y"];
                    pActor->m_Transform.Scale.z = entry["Actor"]["Transform"]["Scale"]["z"];


                    AddActor(pActor);
                }
            }
        }
    }
    
}