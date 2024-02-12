#include "EBO.h"

#include <glad/glad.h>

namespace HKCR {
	EBO::EBO()
	{
		glGenBuffers(1, &m_id);
	}

	void EBO::fillBuffer(const size_t dataSize, const void* data) const
	{
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		unbind();
	}

	void EBO::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void EBO::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
