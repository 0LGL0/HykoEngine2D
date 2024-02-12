#pragma once

#include "Hyko.h"

#include <spdlog/logger.h>

#include "Renderer/Texture/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HKCR {
	class FBO {
	public:
		FBO() = default;
		FBO(const uint16_t width, const uint16_t height);

		HK::Texture createRGBTexture(const uint8_t numOfChannels, const unsigned int imageFormat, const unsigned int dataFormat);
		HK::Texture createDepthTexture(const uint8_t numOfChannels, const unsigned int imageFormat, const unsigned int dataFormat);

		[[nodiscard]]
		const uint16_t getWidth() const;
		[[nodiscard]]
		const uint16_t getHeight() const;
		[[nodiscard]]
		const GLuint getRGB() const;
		[[nodiscard]]
		const GLuint getFBO() const;

		void setWidth(const uint16_t& width);
		void setHeight(const uint16_t& height);

		void updateSize(const uint16_t& newWidth, const uint16_t newHeight);

		void bind() const;
		void unbind() const;
	private:
		void initFBO(const uint16_t width, const uint16_t height);

		GLuint m_fbo;
		GLuint m_RGB;
		GLuint m_depth;

		uint16_t m_width;
		uint16_t m_height;

		HK::Texture m_rgbTexture;
		HK::Texture m_depthTexture;

		uint8_t m_rgbNumOfChannels;
		uint8_t m_depthNumOfChannels;

		unsigned int m_rgbTImageF;
		unsigned int m_rgbTDataF;
		unsigned int m_depthTImageF;
		unsigned int m_depthTDataF;
	};
}