#include "FBO.h"

#include "Scene/Scene.h"
#include "Renderer/Texture/TextureManager.h"

#include "Tools/Log/Logger.h"

// TODO: MASSIVE CODE REMAKING

namespace HKCR {
	FBO::FBO(const uint32_t width, const uint32_t height) {
		initFBO(width, height);
	}

	FBO::~FBO()
	{
		delete m_depthBuff;
		deleteFBO();
	}

	HK::Texture FBO::createRGBTexture(const uint8_t numOfChannels, const unsigned int imageFormat, const unsigned int dataFormat)
	{
		m_rgbNumOfChannels = numOfChannels;
		m_rgbTImageF = imageFormat;
		m_rgbTDataF = dataFormat;

		m_rgbTexture = TextureManager::create(m_width, m_height, { NEREAST_FILTER, NEREAST_FILTER, REPEAT_WRAP, REPEAT_WRAP }, true, numOfChannels, imageFormat, dataFormat, nullptr);
		attachRGBTexture(m_rgbTexture.textureID);
		HK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer was not compiled");

		//m_fbos.push_back(this);

		return m_rgbTexture;
	}

	void FBO::createDepthTexture(const unsigned int format, const unsigned int attachmentFormat) {
		/*m_depthNumOfChannels = numOfChannels;
		m_depthTImageF = imageFormat;
		m_depthTDataF = dataFormat;

		m_depthTexture = TextureManager::create(m_width, m_height, { NEREAST_FILTER, NEREAST_FILTER, REPEAT_WRAP, REPEAT_WRAP }, true, numOfChannels, imageFormat, dataFormat, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.textureID, 0);
		HK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer was not compiled");*/

		m_depth = m_depthBuff->getRBO();

		m_depthBuff->fillBuffer(format);
		attachDepthTexture(DEPTH_COMPONENT, m_depth);
	}

	void FBO::initFBO(const uint32_t width, const uint32_t height) {
		m_width = width;
		m_height = height;

		m_depthBuff = new RBO{ width, height };

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

	void FBO::deleteFBO()
	{
		glDeleteFramebuffers(1, &m_fbo);
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

	const HK::Texture FBO::getRGBTexture() const
	{
		return m_rgbTexture;
	}

	void FBO::setWidth(const uint32_t& width)
	{
		HK_LOG_INFO("The frame buffer width has been updated from {} to {}", m_width, width);
		m_width = width;
	}

	void FBO::setHeight(const uint32_t& height)
	{
		HK_LOG_INFO("The frame buffer height has been updated from {} to {}", m_height, height);
		m_height = height;
	}

	void FBO::attachDepthTexture(const unsigned int attachmentFormat, const GLuint depthID) 
	{
		glNamedFramebufferRenderbuffer(m_fbo, attachmentFormat, GL_RENDERBUFFER, depthID);
		m_depth = depthID;
	}

	void FBO::attachRGBTexture(const GLuint rgbTextureID)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rgbTextureID, 0);
		m_RGB = rgbTextureID;
	}

	void FBO::updateSize(const uint32_t& newWidth, const uint32_t newHeight, const unsigned int depthFormat)
	{
		delete m_depthBuff;

		m_rgbTexture.deleteTexture();
		deleteFBO();

		initFBO(newWidth, newHeight);

		bind();

		createRGBTexture(m_rgbNumOfChannels, m_rgbTImageF, m_rgbTDataF);
		m_depthBuff->resize(newWidth, newHeight, depthFormat);

		unbind();

		m_depth = m_depthBuff->getRBO();

		HK_LOG_INFO("FBO texture was resized (new size: {}x{})", newWidth, newHeight);
	}

	void FBO::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		m_depthBuff->bind();
	}

	void FBO::unbind() const
	{
		m_depthBuff->unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}