#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

#include "../EngineAPI.hpp"
#include "../Utility/UUID.hpp"

namespace Theater
{
	class Actor;

	namespace Detail
	{
		using ActorNewInstanceFn = std::function<Actor* ()>;

		struct ActorClassData
		{
			uuid ClassUUID;
			std::string ClassName;
			ActorNewInstanceFn NewInstanceFn;
		};

		using ActorClassDataMap = std::unordered_map<uuid, ActorClassData>;

		class ENGINE_API ActorRegistry final
		{
		public:

			static ActorRegistry* Get()
			{
				if (!s_pInstance)
					s_pInstance = new ActorRegistry;

				return s_pInstance;
			}

			static void Destroy()
			{
				s_pInstance->m_Registry.clear();

				if (s_pInstance)
				{
					delete s_pInstance;
					s_pInstance = nullptr;
				}
			}

		public:

			void Register(const ActorClassData& data);

		public:

			Actor* CreateInstance(uuid ClassUUID);
			std::vector<const ActorClassData*>* GetActorsClassData() const;
			std::string GetClassNameFromClassUUID(uuid classUUID) const;

		private:

			ActorRegistry() = default;
			~ActorRegistry() = default;

		private:

			ActorClassDataMap m_Registry;
			inline static ActorRegistry* s_pInstance = nullptr;
		};
	}
}


#define THT_REGISTER_ACTORCLASS(ClassName, ClassUUID) \
    namespace { \
        struct Theater_##ClassName##_Register  { \
            Theater_##ClassName##_Register() {\
               Theater::Detail::ActorRegistry::Get()->Register(Theater::Detail::ActorClassData{ClassUUID, #ClassName,  [](){ return new ClassName; }});\
            }\
        };\
    }\
    static Theater_##ClassName##_Register Theater_##ClassName##_Register_Object;