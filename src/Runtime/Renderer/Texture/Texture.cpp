#include "Texture.h"

#include "Platform/GL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture : public HKCR::ITexture {
public:
	void init() override {
		HKCR::loadGLFunctions();
	}

	const unsigned int create(const std::string& pathToTexture, uint32_t* width, uint32_t* height) override {
		GLenum imageFormat;
		GLenum dataFormat;

		stbi_set_flip_vertically_on_load(true);

		m_srcTexture = stbi_load(pathToTexture.c_str(), &m_width, &m_height, &m_channels, 0);

		*width = m_width;
		*height = m_height;

		if (m_channels == 4) {
			imageFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_channels == 3) {
			imageFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		if (m_srcTexture) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_glTexture);
			glTextureStorage2D(m_glTexture, 1, imageFormat, m_width, m_height);

			glTextureParameteri(m_glTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_glTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_glTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_glTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_glTexture, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, m_srcTexture);
		}

		return m_glTexture;
	}

	void free() const override {
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(m_srcTexture);
	}

	void _deleteTexture() override {
		delete this;
	}
private:
	unsigned char *m_srcTexture;
	GLuint m_glTexture;

	int m_width;
	int m_height;
	int m_channels;
};

DLL_EXPORT HKCR::ITexture* _createTexture() {
	return new Texture;
}