#include "ROs.h"

#include "Platform/GL.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace HKCR {
	ROs::ROs() {
		glCreateVertexArrays(1, &m_vaoID);
		glCreateBuffers(1, &m_vboID);
		glCreateBuffers(1, &m_eboID);
	}

	ROs::~ROs() {
		glDeleteVertexArrays(1, &m_vaoID);
		glDeleteBuffers(1, &m_vboID);
		glDeleteBuffers(1, &m_eboID);
	}

	void ROs::pushToGPU(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
		glBindVertexArray(m_vaoID);
		glNamedBufferData(m_vboID, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glNamedBufferData(m_eboID, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glVertexArrayVertexBuffer(m_vaoID, 0, m_vboID, 0, 8 * sizeof(float));
		glVertexArrayElementBuffer(m_vaoID, m_eboID);

		// Vertices position
		glEnableVertexArrayAttrib(m_vaoID, 0);
		glVertexArrayAttribFormat(m_vaoID, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_vaoID, 0, 0);

		// Texture coordinates
		glEnableVertexArrayAttrib(m_vaoID, 1);
		glVertexArrayAttribFormat(m_vaoID, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);
		glVertexArrayAttribBinding(m_vaoID, 1, 0);

		// Object color
		glEnableVertexArrayAttrib(m_vaoID, 2);
		glVertexArrayAttribFormat(m_vaoID, 2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (2 + 2));
		glVertexArrayAttribBinding(m_vaoID, 2, 0);

		// texture ID index
		glEnableVertexArrayAttrib(m_vaoID, 3);
		glVertexArrayAttribFormat(m_vaoID, 3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * (2 + 2 + 3));
		glVertexArrayAttribBinding(m_vaoID, 3, 0);

		glBindVertexArray(0);
	}

	void ROs::bind() const {
		glBindVertexArray(m_vaoID);
	}

	void ROs::unbind() const {
		glBindVertexArray(0);
	}
}
