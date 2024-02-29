#include "TextureManager.h"

#include "Tools/Log/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

#define LOGGER_NAME "TextureLoaderLogger"

namespace HKCR {
	std::unordered_set<GLuint64> TextureManager::m_textureHandles;

	const HK::Texture TextureManager::createFromFile(const std::string& pathToTexture, int* width, int* height, const HK::TextureProps& props, const bool flipY, const bool isRectanglePrimitive) {
		GLuint glTexture;
		int channels;

		stbi_set_flip_vertically_on_load(flipY);

		unsigned char* srcTexture = stbi_load(pathToTexture.c_str(), width, height, &channels, 0);

		int Width = *width;
		int Height = *height;

		if (srcTexture) {
			GLenum imageFormat = GL_RGBA8;
			GLenum dataFormat = GL_RGBA;

			switch (channels) {
			case 3:
				imageFormat = GL_RGB8;
				dataFormat = GL_RGB;
				break;
			case 4:
				imageFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;
			case 1:
				imageFormat = GL_R8;
				dataFormat = GL_RED;
			default:
				HK_LOG_WARNING("{} channels are not supported by the engine", channels);
			}

			const auto texture = create(Width, Height, props, isRectanglePrimitive, channels, imageFormat, dataFormat, srcTexture);
			stbi_image_free(srcTexture); // Free image bytes

			return texture;
		}
		else {
			HK_LOG_ERROR("The texture was not loaded. Path: {}", pathToTexture);
			std::abort();
		}

		return HK::Texture();
	}

	const HK::Texture TextureManager::create(unsigned int width, unsigned int height, const HK::TextureProps& props, const bool isRectanglePrimitive, uint8_t channels, const unsigned int imageFormat, const unsigned int dataFormat, const void* textureData)
	{
		if (channels > 4 || channels == 2) {
			HK_LOG_WARNING("{} channels are not supported by the engine", channels);
			channels = 4;
		}

		GLuint textureID;

		uint64_t handle = 0;

		// Alignment texture memory up to 1 byte
		// This is necessary for correct rendering of images with any aspect ratio
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, imageFormat, width, height);
		if(textureData) 
			glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, textureData);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, props.textureMinFilter);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, props.textureMagFilter);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, props.textureWrapS);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, props.textureWrapT);

		handle = createTextureHandle(textureID);
		makeHandleResident(handle);

		return HK::Texture(HK::UV(isRectanglePrimitive, width, height), textureID, width, height, channels, handle);
	}

	void TextureManager::removeTextureHandle(const unsigned int textureID)
	{
		m_textureHandles.erase(textureID);
	}

	void TextureManager::makeHandleResident(const uint64_t handle)
	{
		glMakeTextureHandleResidentARB(handle);
	}

	void TextureManager::makeHandleNonResident(const uint64_t handle)
	{
		glMakeTextureHandleNonResidentARB(handle);
	}

	void TextureManager::makeAllHandelesNonResident()
	{
		for (const auto& handle : m_textureHandles)
			makeHandleNonResident(handle);
	}

	const uint64_t HKCR::TextureManager::getTextureHandle(const unsigned int textureID)
	{
		return *m_textureHandles.find(textureID);
	}
	const uint64_t HKCR::TextureManager::createTextureHandle(const unsigned int textureID)
	{
		return glGetTextureHandleARB(textureID);
	}
}
