#include "FBO.h"
#include "FBO.h"

#include "Renderer/Texture/TextureManager.h"

#include "Tools/Log/Logger.h"

namespace HKCR {
	FBO::FBO(const uint16_t width, const uint16_t height) {
		initFBO(width, height);
	}

	HK::Texture FBO::createRGBTexture(const uint8_t numOfChannels, const unsigned int imageFormat, const unsigned int dataFormat)
	{
		m_rgbNumOfChannels = numOfChannels;
		m_rgbTImageF = imageFormat;
		m_rgbTDataF = dataFormat;

		m_rgbTexture = TextureManager::create(m_width, m_height, { NEREAST_FILTER, NEREAST_FILTER, REPEAT_WRAP, REPEAT_WRAP }, true, numOfChannels, imageFormat, dataFormat, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rgbTexture.textureID, 0);
		HK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer was not compiled");

		return m_rgbTexture;
	}

	HK::Texture FBO::createDepthTexture(const uint8_t numOfChannels, const unsigned int imageFormat, const unsigned int dataFormat) {
		m_depthNumOfChannels = numOfChannels;
		m_depthTImageF = imageFormat;
		m_depthTDataF = dataFormat;

		m_depthTexture = TextureManager::create(m_width, m_height, { NEREAST_FILTER, NEREAST_FILTER, REPEAT_WRAP, REPEAT_WRAP }, true, numOfChannels, imageFormat, dataFormat, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.textureID, 0);
		HK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer was not compiled");

		return m_depthTexture;
	}

	void FBO::initFBO(const uint16_t width, const uint16_t height) {
		m_width = width;
		m_height = height;

		glCreateFramebuffers(1, &m_fbo);

		/*glCreateTextures(GL_TEXTURE_2D, 1, &m_RGB);
		glBindTexture(GL_TEXTURE_2D, m_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RGB, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth);
		glBindTexture(GL_TEXTURE_2D, m_depth);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_width, m_height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

		HK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer was not compiled");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
	}

	const uint16_t FBO::getWidth() const
	{
		return m_width;
	}

	const uint16_t FBO::getHeight() const
	{
		return m_height;
	}

	const GLuint FBO::getRGB() const
	{
		return m_RGB;
	}

	const GLuint FBO::getFBO() const
	{
		return m_fbo;
	}

	void FBO::setWidth(const uint16_t& width)
	{
		HK_LOG_INFO("The frame buffer width has been updated from {} to {}", m_width, width);
		m_width = width;
	}

	void FBO::setHeight(const uint16_t& height)
	{
		HK_LOG_INFO("The frame buffer height has been updated from {} to {}", m_height, height);
		m_height = height;
	}

	void FBO::updateSize(const uint16_t& newWidth, const uint16_t newHeight)
	{
		m_rgbTexture.deleteTexture();
		m_depthTexture.deleteTexture();

		createRGBTexture(m_rgbNumOfChannels, m_rgbTImageF, m_rgbTDataF);
		createDepthTexture(m_depthNumOfChannels, m_depthTImageF, m_depthTDataF);
	}

	void FBO::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void FBO::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}