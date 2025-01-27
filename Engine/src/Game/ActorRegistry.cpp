#include <Theater/Engine/Game/ActorRegistry.hpp>

namespace Theater
{
	namespace Detail
	{
		void ActorRegistry::Register(const ActorClassData& data)
		{
			m_Registry.insert(
				{
					data.ClassUUID,
					data
				}
			);
		}

		Actor* ActorRegistry::CreateInstance(uuid ClassUUID)
		{
			const auto It = m_Registry.find(ClassUUID);
			if (It != m_Registry.end())
			{
				return It->second.NewInstanceFn();
			}

			return nullptr;
		}

		std::vector<const ActorClassData*>* ActorRegistry::GetActorsClassData() const
		{
			std::vector<const ActorClassData*>* pData = new std::vector<const ActorClassData*>();
			for (const auto& val : m_Registry)
			{
				pData->push_back(&val.second);
			}

			return pData;
		}

		std::string ActorRegistry::GetClassNameFromClassUUID(uuid classUUID) const
		{
			for (const auto& val : m_Registry)
			{
				if (val.first == classUUID)
				{
					return val.second.ClassName;
				}
			}

			return std::string();
		}
	}
}