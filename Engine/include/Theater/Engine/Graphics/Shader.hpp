#pragma once

#include "../Utility//String.hpp"
#include "../EngineAPI.hpp"

namespace Theater
{
	class ENGINE_API Shader
	{
	public:

		Shader();
		~Shader();

		Shader(const Shader&) = delete;
		Shader& operator = (const Shader&) = delete;

		Shader(Shader&& other) noexcept
		{
			*this = std::forward<Shader>(other);
		}

		Shader& operator = (Shader&& other) noexcept
		{
			if (this != &other)
			{
				m_vShaderID = other.m_vShaderID;
				m_fShaderID = other.m_fShaderID;
				m_ID = other.m_ID;
				m_Compiled = other.m_Compiled;
			}

			return *this;
		}

	public:

		void Bind();
		void UnBind();

		void Compile(std::string_view vShader, std::string_view fShader);

		template<typename T>
		void SetUniform(uint32_t location, const T& value, bool transpose = false);

		template<typename T>
		void SetUniform(std::string_view location, const T& value, bool transpose = false)
		{
			SetUniform<T>(GetUniformLocation(location), value);
		}

	public:

		std::string GetError();
		bool IsCompiled() const { return m_Compiled; }

		uint32_t GetID() const { return m_ID; }
		uint32_t GetUniformLocation(std::string_view location) const;


	private:

		std::string GetShaderLog(uint32_t shader);

	private:

		uint32_t m_ID;

		uint32_t m_vShaderID;
		uint32_t m_fShaderID;

		bool m_Compiled;
	};
}