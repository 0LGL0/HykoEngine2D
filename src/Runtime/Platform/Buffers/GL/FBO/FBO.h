#pragma once

#include "Hyko.h"

#include <spdlog/logger.h>

#include "Renderer/Texture/Texture.h"
#include "Platform/Buffers/GL/RBO/RBO.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HKCR {
	class FBO {
	public:
		FBO() = delete;
		FBO(const uint32_t width, const uint32_t height);
		~FBO();

		HK::Texture createRGBTexture(const uint8_t numOfChannels, const unsigned int imageFormat, const unsigned int dataFormat);
		void createDepthTexture(const unsigned int format, const unsigned int attachmentFormat);

		[[nodiscard]]
		const uint16_t getWidth() const;
		[[nodiscard]]
		const uint16_t getHeight() const;
		[[nodiscard]]
		const GLuint getRGB() const;
		[[nodiscard]]
		const GLuint getFBO() const;
		[[nodiscard]]
		const HK::Texture getRGBTexture() const;

		void setWidth(const uint32_t& width);
		void setHeight(const uint32_t& height);

		void attachDepthTexture(const unsigned int attachmentFormat, const GLuint depthID);
		void attachRGBTexture(const GLuint rgbTextureID);

		void updateSize(const uint32_t& newWidth, const uint32_t newHeight, const unsigned int depthFormat);

		void bind() const;
		void unbind() const;
	private:
		void initFBO(const uint32_t width, const uint32_t height);
		void deleteFBO();

		RBO *m_depthBuff;

		GLuint m_fbo;
		GLuint m_RGB;
		GLuint m_depth;

		uint16_t m_width;
		uint16_t m_height;

		HK::Texture m_rgbTexture = HK::Texture{};

		uint8_t m_rgbNumOfChannels;

		unsigned int m_rgbTImageF;
		unsigned int m_rgbTDataF;
	};
}