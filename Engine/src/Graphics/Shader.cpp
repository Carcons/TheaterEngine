#include <glad/glad.h>

#include <iostream>
#include <cassert>

#include <Theater/Engine/Math/Mat3.hpp>
#include <Theater/Engine/Math/Mat4.hpp>
#include <Theater/Engine/Graphics/Shader.hpp>

namespace Theater
{
	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const Vector4& vec, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform4f(m_ID, location, vec[0], vec[1], vec[2], vec[3]);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const Vector3& vec, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform3f(m_ID, location, vec[0], vec[1], vec[2]);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const Vector2& vec, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform2f(m_ID, location, vec[0], vec[1]);
	}

	template<>
	ENGINE_API void Shader::SetUniform<Matrix3>(uint32_t location, const Matrix3& mat, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniformMatrix3fv(m_ID, location, 1, transpose, mat.GetFloatPtr());
	}

	template<>
	ENGINE_API void Shader::SetUniform<Matrix4>(uint32_t location, const Matrix4& mat, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniformMatrix4fv(m_ID, location, 1, transpose, mat.GetFloatPtr());
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const float& value, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform1f(m_ID, location, value);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const double& value, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform1d(m_ID, location, value);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const uint32_t& value, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform1ui(m_ID, location, value);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const int32_t& value, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform1i(m_ID, location, value);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const int64_t& value, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform1i(m_ID, location, (int32_t) value);
	}

	template<>
	ENGINE_API void Shader::SetUniform(uint32_t location, const uint64_t& value, bool transpose)
	{
		assert(m_ID && "Shader::m_ID = 0!");
		glProgramUniform1i(m_ID, location, (uint32_t)value);
	}

	Shader::Shader() :
		m_ID(0u),
		m_fShaderID(0u),
		m_vShaderID(0u),
		m_Compiled(0u)
	{}

	Shader::~Shader()
	{
		if (m_ID)
		{
			glDeleteShader(m_ID);
			m_ID = 0u;
		}
	}

	std::string	Shader::GetError()
	{
		std::string error;

		// Check and write compilation status
		{
			int32_t vShaderStatus = 0;
			int32_t fShaderStatus = 0;

			glGetShaderiv(m_vShaderID, GL_COMPILE_STATUS, &vShaderStatus);
			if (vShaderStatus != GL_TRUE)
			{
				error += GetShaderLog(m_vShaderID);
			}

			glGetShaderiv(m_fShaderID, GL_COMPILE_STATUS, &fShaderStatus);
			if (fShaderStatus != GL_TRUE)
			{
				error += GetShaderLog(m_fShaderID);
			}
		}

		// Check linking
		{
			int32_t linkStatus;
			glGetProgramiv(m_ID, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE)
			{
				std::cout << "Linking error\n";
			}
		}

		return error;
	}

	std::string Shader::GetShaderLog(uint32_t shader)
	{
		std::string buf;
		int32_t len = 0;
		int32_t chWritten = 0;

		char* chBuffer = nullptr;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			chBuffer = reinterpret_cast<char*>(std::malloc(len));
			buf.reserve(len);

			glGetShaderInfoLog(shader, len, &chWritten, chBuffer);

			if (chBuffer)
				buf = chBuffer;
		}

		return buf;
	}

	void Shader::Bind()
	{
		if (!m_ID)
			return;

		glUseProgram(m_ID);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	void Shader::Compile(std::string_view vShader, std::string_view fShader)
	{
		if (m_ID)
		{
			m_Compiled = false;
			m_vShaderID = m_fShaderID = 0u;
		}

		const char* const vShaderSource = vShader.data();
		const char* const fShaderSource = fShader.data();

		m_vShaderID = glCreateShader(GL_VERTEX_SHADER);
		m_fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(m_vShaderID, 1, &vShaderSource, nullptr);
		glShaderSource(m_fShaderID, 1, &fShaderSource, nullptr);
		glCompileShader(m_vShaderID);
		glCompileShader(m_fShaderID);

		// Check compilation error
		{
			int32_t vShaderStatus;
			int32_t fShaderStatus;
			glGetShaderiv(m_vShaderID, GL_COMPILE_STATUS, &vShaderStatus);
			glGetShaderiv(m_vShaderID, GL_COMPILE_STATUS, &fShaderStatus);

			if (vShaderStatus != GL_TRUE)
				return;
		}

		m_ID = glCreateProgram();

		glAttachShader(m_ID, m_vShaderID);
		glAttachShader(m_ID, m_fShaderID);
		glLinkProgram(m_ID);

		// Check linking error
		{
			int32_t linkStatus;
			glGetProgramiv(m_ID, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE)
				return;
		}

		m_Compiled = true;
	}

	uint32_t Shader::GetUniformLocation(std::string_view location) const
	{
		return glGetUniformLocation(m_ID, location.data());
	}
}