#include "VBO.h"

#include <glad/glad.h>

namespace HKCR {
	VBO::VBO()
	{
		glGenBuffers(1, &m_id);
	}

	void VBO::fillBuffer(const size_t dataSize, const void* data) const
	{
		bind();
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		unbind();
	}

	void VBO::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VBO::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
