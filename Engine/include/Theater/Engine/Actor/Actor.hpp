#pragma once

#include <unordered_set>
#include <memory>
#include <string>
#include <vector>

#include "../EngineAPI.hpp"
#include "../Utility/UUID.hpp"

#include "Transform.hpp"
#include "../Graphics/Model.hpp"

namespace Theater
{
	namespace Detail
	{
		class ActorRegistry;
	}
}

namespace Theater
{
	class Shader;

	class ENGINE_API Actor
	{
		friend class Game;

	public:

		Transform m_Transform;
		Model m_Model;

		bool m_Alive{ true };

	public:

		Actor();
		virtual ~Actor();

		Actor(const Actor&) = delete;
		Actor& operator = (const Actor&) = delete;

		Actor(Actor&& other) noexcept
		{
			*this = std::forward<Actor>(other);
		}

		Actor& operator = (Actor&& other) noexcept
		{
			if (this != &other)
			{
				m_Alive = other.m_Alive;
				m_UUID = other.m_UUID;

				other.m_Alive = false;
				other.m_UUID = 0;
			}
			return *this;
		}

	public:

		uuid GetUUID() const { return m_UUID; }

	public:

		virtual void OnUpdate(float deltaTime);
		virtual void OnDraw(Shader& shader);

		virtual void OnGameReady() {}

	private:
		uuid m_UUID;
	};
}