#pragma once 

#include "Hyko.h"

#include <spdlog/logger.h>

#include <glm/glm.hpp>

using GLuint = unsigned int;

namespace HKCR {
	class Shader {
	public:
		void loadVertexShader(const std::string& pathToShader);
		void loadFragmentShader(const std::string& pathToShader);

		void compileVertexShader();
		void compileFragmentShader();
		const unsigned int compileShaderProgram();

		const bool setUniform(const std::string& uName, const glm::mat4& matrix);
		const bool setUniform(const std::string& uName, const unsigned int uInt);
		const bool setUniform(const std::string& uName, const int _int);
		const bool setUniform(const std::string& uName, const float _float);
		const bool setUniform(const std::string& uName, const std::vector<unsigned int>& uIntV);
		const bool setUniform(const std::string& uName, const std::vector<float>& floatV);
		const bool setUniform(const std::string& uName, const glm::vec3& vec3);
		const bool setUniform(const std::string& uName, const uint64_t _uInt64);

		void bind() const;
		void unbind() const;
	private:
		const bool getUniformLoc(const std::string& uName, uint16_t* loc);
		void loadShader(std::string* shaderSrcVar, const std::string& pathToShader);
		void compileShader(unsigned int* shaderIDVar, const int shaderType, const char* shaderSrc);
		void isCompiledShader(const unsigned int shaderID, const int shaderType);

		GLuint m_vertexShaderID = 0;
		GLuint m_fragmentShaderID = 0;
		GLuint m_shaderProgram = 0;

		std::string m_vertexShaderSrc;
		std::string m_fragmentShaderSrc;

		std::unordered_map<std::string, uint16_t> m_uniforms;
	};
}
