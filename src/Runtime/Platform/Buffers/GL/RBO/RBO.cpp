#include "RBO.h"
#include "RBO.h"

#include "Tools/Log/Logger.h"

#include <glad/glad.h>

namespace HKCR {
	RBO::RBO(const uint32_t width, const uint32_t height)
	{
		init(width, height);
	}

	RBO::~RBO()
	{
		deleteRBO();
	}

	void RBO::fillBuffer(const unsigned int format) const
	{
		glNamedRenderbufferStorage(m_id, format, m_width, m_height);
	}

	void RBO::resize(const uint32_t newWidth, const uint32_t newHeight, const unsigned int newFormat)
	{
		m_width = newWidth;
		m_height = newHeight;

		glDeleteRenderbuffers(1, &m_id);

		init(newWidth, newHeight);
		fillBuffer(newFormat);

		HK_LOG_INFO("RBO texture was resized (new size: {}x{})", newWidth, newHeight);
	}

	void RBO::bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	}

	void RBO::unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	const GLuint RBO::getRBO() const
	{
		return m_id;
	}

	void RBO::init(const uint32_t width, const uint32_t height)
	{
		glGenRenderbuffers(1, &m_id);

		m_width = width;
		m_height = height;
	}

	void HKCR::RBO::deleteRBO()
	{
		glDeleteRenderbuffers(1, &m_id);
	}
}
