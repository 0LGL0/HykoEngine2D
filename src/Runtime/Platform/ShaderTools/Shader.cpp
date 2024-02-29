#include "Shader.h"

#include "Platform/GL.h"
#include "Tools/Log/Logger.h"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#define LOGGER_NAME "ShaderLogger"

namespace HKCR {
	void Shader::loadVertexShader(const std::string& pathToShader) {
		loadShader(&m_vertexShaderSrc, pathToShader);
	}

	void Shader::loadFragmentShader(const std::string& pathToShader) {
		loadShader(&m_fragmentShaderSrc, pathToShader);
	}

	void Shader::compileVertexShader() {
		compileShader(&m_vertexShaderID, GL_VERTEX_SHADER, m_vertexShaderSrc.c_str());
	}

	void Shader::compileFragmentShader() {
		compileShader(&m_fragmentShaderID, GL_FRAGMENT_SHADER, m_fragmentShaderSrc.c_str());
	}

	const unsigned int Shader::compileShaderProgram() {
		m_shaderProgram = glCreateProgram();

		if (m_vertexShaderID) {
			glAttachShader(m_shaderProgram, m_vertexShaderID);
		}
		if (m_fragmentShaderID) {
			glAttachShader(m_shaderProgram, m_fragmentShaderID);
		}

		glLinkProgram(m_shaderProgram);
		isCompiledShader(m_shaderProgram, GL_PROGRAM);

		glDetachShader(m_shaderProgram, m_vertexShaderID);
		glDetachShader(m_shaderProgram, m_fragmentShaderID);

		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		return m_shaderProgram;
	}

	void Shader::compileShader(unsigned int* shaderIDVar, const int shaderType, const char* shaderSrc) {
		*shaderIDVar = glCreateShader(shaderType);
		glShaderSource(*shaderIDVar, 1, &shaderSrc, NULL);
		glCompileShader(*shaderIDVar);

		isCompiledShader(*shaderIDVar, shaderType);
	}

	void Shader::loadShader(std::string* shaderSrcVar, const std::string& pathToShader) {
		std::ifstream shaderFile{ pathToShader };
		HK_ASSERT(shaderFile.is_open(), "Shader file ({}) was not open", pathToShader.c_str());

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		*shaderSrcVar = shaderStream.str();
	}

	void Shader::isCompiledShader(const unsigned int shaderID, const int shaderType) {
		GLint success;
		char infolog[512];

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderID, 512, NULL, infolog);
			HK_LOG_ERROR("Shader({}) not compilled {}", shaderType, infolog);
			abort();
		}
	}

	void Shader::bind() const {
		glUseProgram(m_shaderProgram);
	}

	void Shader::unbind() const {
		glUseProgram(0);
	}


	// Matrix =========================================================

	const bool Shader::setUniform(const std::string& uName, const glm::mat4& matrix) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
			return true;
		}
		return false;
	}

	// unsigned int ===================================================

	const bool Shader::setUniform(const std::string& uName, const unsigned int uInt) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform1ui(loc, uInt);
			return true;
		}
		return false;
	}

	const bool Shader::setUniform(const std::string& uName, const uint64_t _uInt64) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform1ui64ARB(loc, _uInt64);
			return true;
		}
		return false;
	}

	const bool Shader::setUniform(const std::string& uName, const std::vector<unsigned int>& uIntV) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform1uiv(loc, uIntV.size(), uIntV.data());
			return true;
		}
		return false;
	}

	// int ==============================================================

	const bool Shader::setUniform(const std::string& uName, const int _int) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform1i(loc, _int);
			return true;
		}
		return false;
	}

	// float ============================================================

	const bool Shader::setUniform(const std::string& uName, const float _float) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform1f(loc, _float);
			return true;
		}
		return false;
	}

	const bool Shader::setUniform(const std::string& uName, const std::vector<float>& floatV) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform1fv(loc, floatV.size(), floatV.data());
			return true;
		}
		return false;
	}

	// glsl vector =======================================================

	const bool Shader::setUniform(const std::string& uName, const glm::vec3& vec3) {
		uint16_t loc;
		if (getUniformLoc(uName, &loc)) {
			glUniform3f(loc, vec3.x, vec3.y, vec3.z);
			return true;
		}
		return false;
	}

	const bool Shader::getUniformLoc(const std::string& uName, uint16_t* loc) {
		if (const auto it = m_uniforms.find(uName); it != m_uniforms.end()) {
			*loc = it->second;
			return true;
		}

		*loc = glGetUniformLocation(m_shaderProgram, uName.c_str());
		if (*loc == static_cast<uint16_t>(-1)) {
			HK_LOG_ERROR("Uniform '{}' not found", uName);
			return false;
		}

		m_uniforms.insert({ uName, *loc });
		return true;
	}
}