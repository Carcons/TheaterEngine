#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _WIN32

#include "Montserrat-Regular.hpp"

#include <TheaterEditor/Editor.hpp>
#include <Theater/Engine/Platform/WindowSystem.hpp>
#include <Theater/Engine/Graphics/RenderingSystem.hpp>

#include <imgui_impl_opengl3.h>

#if defined(_WIN32)
#include <imgui_impl_win32.h>
#endif
#include <imgui_internal.h>
#include <imgui.h>

#define ImGuiCenterNextWin() ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f))

#include <json.hpp>
using json = nlohmann::json;

#include <tinyfiledialogs.h>

#include <filesystem>
namespace stdfs = std::filesystem;

#include <inttypes.h>

#include <iostream>
#include <list>
#include <algorithm>
#include <fstream>

#include <Theater/Engine/Math/Mat4.hpp>
#include <Theater/Engine/Platform/ILibrary.hpp>
#include <Theater/Engine/Platform/IProcess.hpp>
#include <Theater/Engine/Platform/WindowSystem.hpp>
#include <Theater/Engine/Utility/String.hpp>
#include <Theater/Engine/Actor/Actor.hpp>
#include <Theater/Engine/Platform/WinSysEvents.hpp>
#include <Theater/Engine/Utility/UUID.hpp>
#include <Theater/GameplayAPI/GameLibInterface.hpp>
#include <Theater/Engine/Graphics/Texture.hpp>
#include <Theater/GameplayAPI/TheaterGameplay.hpp>

using namespace Theater;
using namespace Theater::Detail;

#pragma region ImGuiPlatform
namespace Editor
{
	namespace
	{
		void ImGui_PlatformInit()
		{
#if defined(_WIN32)

			::ImGui_ImplWin32_Init(WindowSystem::Get().GetNativeWinPtr());
			WindowSystem::Get().InstallWin32WndProc([](void* hWnd, uint32_t msg, void* wParam, void* lParam) -> bool
				{
					extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
					return ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, (WPARAM)wParam, (LPARAM)lParam);
				});

#endif
		}

		void ImGui_PlatformNewFrame()
		{
#if defined(_WIN32)
			::ImGui_ImplWin32_NewFrame();
#endif
		}

		void ImGui_PlatformShutdown()
		{
#if defined(_WIN32)
			::ImGui_ImplWin32_Shutdown();
#endif
		}
	}
}
#pragma endregion

#pragma region Classes&Data
namespace Editor
{
	enum EditorWindow : uint16_t
	{
		EditorWindow_Null = 0,
		EditorWindow_AssetsImporter = 1
	};

	struct GameState
	{
		TexturedQuad GameViewQuad;
		json* pGameData;
		ILibrary* pGameLib;
		std::vector<const ActorClassData*> ActorsClassData;
		GameInterfaceFunctions GameFunctions;

		GameState()
		{
			pGameData = nullptr;
			pGameLib = NewLibrary();

			GameViewQuad.m_VertexData[0] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } }; // Bottom-left
			GameViewQuad.m_VertexData[1] = { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } }; // Bottom-right
			GameViewQuad.m_VertexData[2] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } }; // Top-left

			GameViewQuad.m_VertexData[3] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } }; // Top-left
			GameViewQuad.m_VertexData[4] = { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } }; // Bottom-right
			GameViewQuad.m_VertexData[5] = { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } }; // Top-right
			GameViewQuad.Create();
			assert(GameViewQuad.GetVAO());
		}

		~GameState()
		{
			DeleteLibrary(pGameLib);
		}

		bool IsValid() const
		{
			return pGameLib->IsLoaded();
		}
	};


	namespace
	{
		uint16_t OpenWindows = EditorWindow_Null;
		Matrix4 GameViewProjMat;
		Matrix4 GameViewViewMat;
		ImVec2 GameViewportSize;
		RenderingTarget GameViewportTarget;

		GameState* pState = nullptr;
		const stdfs::path CurrentPath = stdfs::current_path();
		const stdfs::path BinaryPath = CurrentPath.parent_path() / "Bin";
		const stdfs::path AssetsPath = BinaryPath / "Assets";

		constexpr char* const GameLibName = GAME_LIBRARY;
	}

}
#pragma endregion

#pragma region Functions Decl

namespace Editor
{
#pragma region UI
	namespace
	{
		void ComposeUI();
	}
#pragma endregion

#pragma region Helpers
	namespace
	{
		std::vector<stdfs::path> ListDirectory(const stdfs::path& path);
		std::string OpenFileDialog(std::string_view title, std::initializer_list<std::string_view> ext);
		bool SaveFileDialog(std::string_view title, std::string_view ext);
	}
#pragma endregion

#pragma region Actions
	namespace
	{
		std::string GenUUID();
		const ActorClassData* GetActorClassData(uuid classUUID);
		void NewActorInstance(uuid classUUID);
		void UpdateActorTransform(Actor* pActor, uuid actorUUID);
		Actor* GetActorInstance(uuid actorUUID);
		void DeleteActorInstance(uuid actorUUID);
		void MountGameLibrary();
		void UnMountGameLibrary();

		void LoadActorsClassData();
		void RefreshAssetsList();
	}
#pragma endregion
}

#pragma endregion

#pragma region GameLoop
namespace Editor
{
#pragma region Init

	void Init()
	{
		pState = new GameState;

		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_PlatformInit();
		::ImGui_ImplOpenGL3_Init();

		const float DpiScale = WindowSystem::Get().GetContentScale();

		io.Fonts->Clear();

		constexpr float BaseFontSize = 16.0f;
		const float ScaledFontSize = BaseFontSize * DpiScale;

		io.Fonts->AddFontFromMemoryCompressedTTF(Montserrat_Regular_compressed_data, Montserrat_Regular_compressed_size, ScaledFontSize);

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(DpiScale);
		

		// Load Game lib
		{
			MountGameLibrary();
		}

		GameViewportTarget.Create(1920, 1080, RenderingTarget::TargetType::Texture);
	}

#pragma endregion

#pragma region Update

	void Update()
	{
		ComposeUI();
		if (pState->IsValid())
		{
			GameViewportTarget.Resize(GameViewportSize.x, GameViewportSize.y);
			RenderingSystem::Get().AttachTarget(&GameViewportTarget);
			Vector4 viewport = Vector4(0.f, 0.f, GameViewportSize.x, GameViewportSize.y);
			RenderingSystem::Get().SetViewport(viewport);
			pState->GameFunctions.UpdateScene(ImGui::GetIO().DeltaTime);
			pState->GameFunctions.RenderScene(&GameViewportTarget, &GameViewProjMat, &GameViewViewMat);
			RenderingSystem::Get().DetachCurrentTarget();

			RenderingSystem::Get().ClearColorBuffer();
			pState->GameViewQuad.Draw(*RenderingSystem::Get().GetTexturedQuadShader(), GameViewportTarget.GetTextureID());
		}

	}

#pragma endregion

#pragma region UpdateUI

	void UpdateUI()
	{
		::ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

#pragma endregion

#pragma region Shutdown

	void Shutdown()
	{
		UnMountGameLibrary();

		::ImGui_ImplOpenGL3_Shutdown();
		ImGui_PlatformShutdown();
		ImGui::DestroyContext();

		if (pState)
		{
			delete pState;
			pState = nullptr;
		}
	}

#pragma endregion
}
#pragma endregion

#pragma region Functions Impl
namespace Editor
{
	namespace
	{
#pragma region UI
		void ComposeUI()
		{
			::ImGui_ImplOpenGL3_NewFrame();
			ImGui_PlatformNewFrame();
			ImGui::NewFrame();

			// Check GameLib Status
			{
				if (!pState->IsValid())
				{
					ImGuiCenterNextWin();
					ImGui::OpenPopup("Game library can't be loaded");
					if (ImGui::BeginPopupModal("Game library can't be loaded", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Can't open Gameplay Library");
						if (ImGui::Button("Retry"))
						{
							MountGameLibrary();
						}
						ImGui::EndPopup();
					}

					ImGui::Render();
					return;
				}
			}

			#pragma region MainMenu
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save"))
					{
						pState->GameFunctions.WriteGameData();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Assets"))
				{
					if (ImGui::MenuItem("Assets Browser"))
					{
					}

					if (ImGui::MenuItem("Import Assets"))
					{
					}

					if (ImGui::MenuItem("Build Assets Pack"))
					{

					}

					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("Game"))
				{
					if (ImGui::MenuItem("Play Game"))
					{

					}

					if (ImGui::MenuItem("Stop Game"))
					{

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("GameLibrary"))
				{
					if (ImGui::MenuItem("Generate UUID to Clipboard"))
					{
						static std::string UUIDstr;
						UUIDstr = GenUUID();

						ImGui::SetClipboardText(UUIDstr.c_str());
					}

					if (ImGui::MenuItem("ReMount Gameplay Library"))
					{
						MountGameLibrary();
					}

					if (ImGui::MenuItem("UnMount Gameplay Library"))
					{
						UnMountGameLibrary();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
			#pragma endregion

			if (!pState->IsValid())
			{
				ImGui::Render();
				return;
			}

			#pragma region Dockspace
			ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			ImGuiViewport* const pViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(pViewport->Pos);
			ImGui::SetNextWindowSize(pViewport->Size);
			ImGui::SetNextWindowViewport(pViewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace", nullptr, WindowFlags);
			ImGui::PopStyleVar();
			ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspaceID = ImGui::GetID("CustomDockspace");
				ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

				static auto first_time = true;
				if (first_time)
				{
					first_time = false;

					ImGui::DockBuilderRemoveNode(dockspaceID);
					ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_DockSpace);
					ImGui::DockBuilderSetNodeSize(dockspaceID, pViewport->Size);

					auto dockIDleft = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.2f, nullptr, &dockspaceID);
					auto dockIDdown = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Down, 0.25f, nullptr, &dockspaceID);
					auto dockIDright = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Right, 0.2f, nullptr, &dockspaceID);
					auto dockIDcenter = dockspaceID;

					ImGui::DockBuilderDockWindow("Actors Instances", dockIDleft);
					ImGui::DockBuilderDockWindow("Available Actors", dockIDdown);
					ImGui::DockBuilderDockWindow("Properties", dockIDright);
					ImGui::DockBuilderDockWindow("Game Viewport", dockIDcenter);

					ImGui::DockBuilderFinish(dockspaceID);
				}
			}

			ImGui::End();
			#pragma endregion

			static uuid sPickedActorUUID;
			#pragma region Actors Instances
			{
				ImGui::Begin("Actors Instances", nullptr, ImGuiWindowFlags_NoMove);

				size_t x = 0ull;
				static int32_t sSelectedIndex = -1;
				static bool sRemoveActor;
				static json sRemoveEntry;
				for (auto& entry : *pState->pGameData)
				{
					ImGui::PushID(x);
					if (entry.contains("Actor"))
					{
						const bool IsSelected = (x == sSelectedIndex);
						const std::string& str = entry["Actor"]["ClassName"];
						if (ImGui::Selectable(str.c_str(), IsSelected))
						{
							sSelectedIndex = x;
							sPickedActorUUID = entry["Actor"]["UUID"];
						}

						if (ImGui::BeginPopupContextItem(str.c_str()))
						{
							if (ImGui::MenuItem("Remove"))
							{
								sRemoveActor = true; 
								sRemoveEntry = entry["Actor"]["UUID"];
								sSelectedIndex = -1;
							}
							ImGui::EndPopup();
						}
					}

					ImGui::PopID();
					++x;
				}

				if (sRemoveActor)
				{
					DeleteActorInstance(sRemoveEntry);
					sRemoveEntry.clear();
					sPickedActorUUID = 0;
					sRemoveActor = false;
				}

				ImGui::End();
			}
			#pragma endregion

			#pragma region Available Actors
			ImGui::Begin("Available Actors", nullptr, ImGuiWindowFlags_NoMove);
			for (auto* pClassData : pState->ActorsClassData)
			{
				if (ImGui::Selectable(pClassData->ClassName.c_str()))
				{}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("DND_ACTORINSTANCE", pClassData, sizeof(void*), ImGuiCond_Once);
					ImGui::Text("Dragging %s", pClassData->ClassName.c_str());
					ImGui::EndDragDropSource();
				}
			}
			ImGui::End();
			#pragma endregion

			#pragma region Properties
			ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoMove);
			if (sPickedActorUUID != 0)
			{
				if (Actor* const pActor = GetActorInstance(sPickedActorUUID))
				{
					ImGui::Text("UUID: %llu", pActor->GetUUID());
					ImGui::Separator();

					// Translation
					ImGui::Text("Translation:");
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::InputFloat("##PosX", &pActor->m_Transform.Position.x, 0.1f, 1.0f);

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::InputFloat("##PosY", &pActor->m_Transform.Position.y, 0.1f, 1.0f);

					ImGui::Text("Z");
					ImGui::SameLine();
					ImGui::InputFloat("##PosZ", &pActor->m_Transform.Position.z, 0.1f, 1.0f);

					ImGui::Separator();

					// Rotation
					ImGui::Text("Rotation:");
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::InputFloat("##RotX", &pActor->m_Transform.Rotation.x);

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::InputFloat("##RotY", &pActor->m_Transform.Rotation.y);

					ImGui::Text("Z");
					ImGui::SameLine();
					ImGui::InputFloat("##RotZ", &pActor->m_Transform.Rotation.z);

					ImGui::Separator();

					// Scale
					ImGui::Text("Scale:");
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::InputFloat("##ScaleX", &pActor->m_Transform.Scale.x, 0.1f, 1.0f);

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::InputFloat("##ScaleY", &pActor->m_Transform.Scale.y, 0.1f, 1.0f);

					ImGui::Text("Z");
					ImGui::SameLine();
					ImGui::InputFloat("##ScaleZ", &pActor->m_Transform.Scale.z, 0.1f, 1.0f);
					
					UpdateActorTransform(pActor, sPickedActorUUID);
				}
			}
			ImGui::End();
			#pragma endregion

			#pragma region GameViewport

			ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
			ImGui::BeginChild("Frame");

			GameViewportSize = ImGui::GetContentRegionAvail();
			GameViewProjMat = BuildPerspective(ToRad(60.f), GameViewportSize.x / GameViewportSize.y, 0.1f, 1000.f);
			GameViewViewMat = Translate(IMat4, Vector3(0.f, 0.f, -320.f));
			const uint32_t GameTexID = GameViewportTarget.GetTextureID();
			ImGui::Image((ImTextureID)(intptr_t)GameTexID, GameViewportSize, ImVec2(0,0), ImVec2(1,-1));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* const pPayload = ImGui::AcceptDragDropPayload("DND_ACTORINSTANCE"))
				{
					auto* const pClassData = reinterpret_cast<const ActorClassData*>(pPayload->Data);

					NewActorInstance(pClassData->ClassUUID);
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::EndChild();
			ImGui::End();
			#pragma endregion


			ImGui::Render();
		}

#pragma endregion

#pragma region Helpers

		std::string OpenFileDialog(std::string_view title, std::initializer_list<std::string_view> ext)
		{
			std::vector<const char*> filters;
			for (auto& str : ext)
				filters.push_back(str.data());

			const char* const pFile = ::tinyfd_openFileDialog(
				title.data(),	// Title
				"",              // Default file path
				static_cast<int>(filters.size()), // Number of filters
				filters.data(),  // Array of file extension filters
				nullptr,         // Description for the filters 
				0                // multiple selections 
			);

			if (pFile)
				return std::string(pFile);

			return std::string();
		}

		bool SaveFileDialog(std::string_view title, std::string_view ext)
		{
			return false;
		}

		std::vector<stdfs::path> ListDirectory(const stdfs::path& path)
		{
			std::vector<stdfs::path> filesList;

			if (stdfs::exists(path) && stdfs::is_directory(path))
			{
				for (const auto& entry : stdfs::directory_iterator(path))
				{
					filesList.push_back(entry.path());
				}
			}
			return filesList;
		}

#pragma endregion

#pragma region Actions

		std::string GenUUID()
		{
			const uuid UUID = UUID::GenerateUUID();
			return std::to_string(UUID);
		}

		const ActorClassData* GetActorClassData(uuid classUUID)
		{
			for (const auto* pData : pState->ActorsClassData)
			{
				if (pData->ClassUUID == classUUID)
					return pData;
			}

			return nullptr;
		}

		void NewActorInstance(uuid classUUID)
		{
			Actor* const pActor = reinterpret_cast<Actor*>(pState->GameFunctions.NewActorInstance(classUUID));
			pActor->OnGameReady();

			json entry;
			entry["Actor"] = {
				{"UUID", pActor->GetUUID()},
				{"ClassUUID", classUUID},
				{"ClassName", GetActorClassData(classUUID)->ClassName},
				{"Transform", {
					{"Position", {
						{"x", pActor->m_Transform.Position.x},
						{"y", pActor->m_Transform.Position.y},
						{"z", pActor->m_Transform.Position.z}
					}},
					{"Rotation", {
						{"x", pActor->m_Transform.Rotation.x},
						{"y", pActor->m_Transform.Rotation.y},
						{"z", pActor->m_Transform.Rotation.z}
					}},
					{"Scale", {
						{"x", pActor->m_Transform.Scale.x},
						{"y", pActor->m_Transform.Scale.y},
						{"z", pActor->m_Transform.Scale.z}
					}}
				}}
			};

			pState->pGameData->push_back(entry);
		}

		Actor* GetActorInstance(uuid actorUUID)
		{
			return reinterpret_cast<Actor*>(pState->GameFunctions.GetActorInstance(actorUUID));
		}

		void UpdateActorTransform(Actor* pActor, uuid actorUUID)
		{
			for (auto& entry : *pState->pGameData)
			{
				if (entry.contains("Actor") && entry["Actor"]["UUID"] == actorUUID)
				{
					entry["Actor"]["Transform"]["Position"]["x"] = pActor->m_Transform.Position.x;
					entry["Actor"]["Transform"]["Position"]["y"] = pActor->m_Transform.Position.y;
					entry["Actor"]["Transform"]["Position"]["z"] = pActor->m_Transform.Position.z;

					entry["Actor"]["Transform"]["Rotation"]["x"] = pActor->m_Transform.Rotation.x;
					entry["Actor"]["Transform"]["Rotation"]["y"] = pActor->m_Transform.Rotation.y;
					entry["Actor"]["Transform"]["Rotation"]["z"] = pActor->m_Transform.Rotation.z;

					entry["Actor"]["Transform"]["Scale"]["x"] = pActor->m_Transform.Scale.x;
					entry["Actor"]["Transform"]["Scale"]["y"] = pActor->m_Transform.Scale.y;
					entry["Actor"]["Transform"]["Scale"]["z"] = pActor->m_Transform.Scale.z;

					break;
				}
			}
		}

		void DeleteActorInstance(uuid actorUUID)
		{
			json& data = *pState->pGameData;
			const auto It = std::remove_if(data.begin(), data.end(), 
				[actorUUID](const json& item) 
				{
					return item.contains("Actor") && item["Actor"]["UUID"] == actorUUID;
				}
			);
			if (It != data.end())
			{
				data.erase(It, data.end());
			}

			pState->GameFunctions.DeleteActorInstance(actorUUID);
		}

		void MountGameLibrary()
		{
			UnMountGameLibrary();

			if (!pState->pGameLib->Load(GameLibName))
				return;

			pState->GameFunctions.NewGame = THT_LoadLibFn(pState->pGameLib, void* (), "__Theater_NewGame");
			pState->GameFunctions.FreeGame = THT_LoadLibFn(pState->pGameLib, void (void*), "__Theater_FreeGame");
			pState->GameFunctions.Init = THT_LoadLibFn(pState->pGameLib, int(void*), "Theater_InitGameplay");
			pState->GameFunctions.Shutdown = THT_LoadLibFn(pState->pGameLib, void(), "Theater_ShutdownGameplay");
			pState->GameFunctions.GetGameData = THT_LoadLibFn(pState->pGameLib, void*(), "Theater_GetGameData");
			pState->GameFunctions.WriteGameData = THT_LoadLibFn(pState->pGameLib, void(), "Theater_WriteGameData");
			pState->GameFunctions.NewActorInstance = THT_LoadLibFn(pState->pGameLib, void*(uint64_t), "Theater_NewActorInstance");
			pState->GameFunctions.GetActorInstance = THT_LoadLibFn(pState->pGameLib, void*(uint64_t), "Theater_GetActorInstance");
			pState->GameFunctions.DeleteActorInstance = THT_LoadLibFn(pState->pGameLib, void(uint64_t), "Theater_DeleteActorInstance");
			pState->GameFunctions.RenderScene = THT_LoadLibFn(pState->pGameLib, void(void*, void*, void*), "Theater_RenderScene");
			pState->GameFunctions.UpdateScene = THT_LoadLibFn(pState->pGameLib, void(float), "Theater_UpdateScene");
			pState->GameFunctions.GetActorsClassData = THT_LoadLibFn(pState->pGameLib, void* (), "Theater_GetActorsClassData");
			pState->GameFunctions.FreeActorsClassData = THT_LoadLibFn(pState->pGameLib, void (void*), "Theater_FreeActorsClassData");

			pState->GameFunctions.NewGame();
			pState->GameFunctions.Init(nullptr);

			pState->pGameData = reinterpret_cast<json*>(pState->GameFunctions.GetGameData());
			LoadActorsClassData();

			
		}

		void UnMountGameLibrary()
		{
			if (pState->pGameLib->IsLoaded())
			{
				//pState->GameFunctions.FreeGame();
				pState->GameFunctions.Shutdown();
				pState->pGameLib->Free();

				pState->GameFunctions = {};
				pState->ActorsClassData.clear();
				pState->pGameData = nullptr;
				pState->GameFunctions = {};
			}
		}

		void LoadActorsClassData()
		{
			if (pState->IsValid())
			{
				std::vector<const ActorClassData*>* pData = reinterpret_cast<std::vector<const ActorClassData*>*>(pState->GameFunctions.GetActorsClassData());
				pState->ActorsClassData = *pData;
				pState->GameFunctions.FreeActorsClassData(pData);
			}
		}

		void RefreshAssetsList()
		{
			//pState->AssetsList = ListDirectory(AssetsPath);
		}

#pragma endregion
	}
}
#pragma endregion

#pragma region EntryPoint

#include <thread>
#include <chrono>
using namespace std::chrono_literals;


int main(int argc, char** argv)
{
	WindowSystem* pWinSys = nullptr;
	RenderingSystem* pRenderer = nullptr;

	pWinSys = NewWindowSystem();
	pWinSys->SetDPIAware();

	assert(pWinSys && "Can't create WindowSystem");
	if (!pWinSys)
		return -1;

	// Window
	{
		constexpr uint32_t DefaultWidth = 1920;
		constexpr uint32_t DefaultHeight = 1080;

		uint32_t w = DefaultWidth, h = DefaultHeight;
		const auto Monitors = pWinSys->GetMonitors();
		for (auto& monitor : Monitors)
		{
			if (monitor.IsPrimary)
			{
				w = monitor.Width;
				h = monitor.Height;

				break;
			}
		}

		WindowSpecs winSpecs{};
		winSpecs.Width = w;
		winSpecs.Height = h;
		winSpecs.State = WindowState::Maximized;
		winSpecs.Title = "TheaterEditor";

		pWinSys->ConstructWindow(winSpecs);
		pWinSys->SetVSync(true);
	}

	// Renderer
	pRenderer = new RenderingSystem;

	assert(pRenderer && "Can't create Renderer");
	if (!pRenderer)
		return -2;

	Editor::Init();

	while (pWinSys->IsOpen())
	{
		pWinSys->ProcessEvents();

		pRenderer->ClearAllBuffers();

		Editor::Update();
		Editor::UpdateUI();

		pWinSys->SwapBuffers();

		std::this_thread::sleep_for(1ms);
	}

	Editor::Shutdown();

	delete pRenderer;
	pRenderer = nullptr;

	DeleteWindowSystem(pWinSys);
	pWinSys = nullptr;

	return 0;
}
#pragma endregion
