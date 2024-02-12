#pragma once

#include "Hyko.h"
#include "Renderer/UV/UV.h"

#include <glad/glad.h>

namespace HKCR {
	class Renderer;
}

namespace HK {
	struct Texture {
		HK::UV uv;

		GLuint textureID = -1;

		uint32_t width;
		uint32_t height;

		uint8_t channels;

		Texture() = default;
		Texture(const HK::UV& _uv, const GLuint _textureID, const uint32_t _width, const uint32_t _height, uint8_t _channels, uint64_t handle)
			: uv(_uv), textureID(_textureID), width(_width), height(_height), channels(_channels), m_handle(handle) {}

		inline const bool deleteTexture() {
			if (textureID == -1)
				return false;

			glMakeTextureHandleNonResidentARB(m_handle);

			glDeleteTextures(1, &textureID);
			textureID = static_cast<GLuint>(-1);
			width = 0;
			height = 0;
			channels = 0;
		}
	private:
		uint64_t m_handle = 0;

		friend HKCR::Renderer;
	};
}