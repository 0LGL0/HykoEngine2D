#pragma once

#include "Hyko.h"
#include "Texture.h"

#define NEREAST_FILTER				0x2600		// GL_NEAREST
#define LINEAR_FILTER				0x2601		// GL_LINEAR

#define REPEAT_WRAP					0x2901		// GL_REPEAT
#define MIRRORED_REPEAT_WRAP		0x8370		// GL_MIRRORED_REPEAT
#define CLAMP_TO_EDGE_WRAP			0x812F		// GL_CLAMP_TO_EDGE
#define CLAMP_TO_BORDER_WRAP		0x812D		// GL_CLAMP_TO_BORDER

#define RED							0x1903		// GL_RED
#define R8							0x8229		// GL_R8
#define R16							0x822A		// GL_R16
#define R16F						0x822B		// GL_R16F
#define R32F						0x822E		// GL_RG32F
#define R8I							0x8231		// GL_R8I
#define R8UI						0x8232		// GL_R8UI
#define R16I						0x8233		// GL_R16I
#define R16UI						0x8234		// GL_R16UI
#define R32I						0x8235		// GL_R32I
#define R32UI						0x8236		// GL_R32UI
#define RG8I						0x8237		// GL_RG8I
#define RG8UI						0x8238		// GL_RG8UI
#define RG16I						0x8239		// GL_RG16I
#define RG16UI						0x823A		// GL_RG16UI
#define RG32I						0x823B		// GL_RG32I
#define RG32UI						0x823C		// GL_RG32UI

#define RGB							0x1907		// GL_RGB
#define RGB4						0x804F		// GL_RGB4
#define RGB5						0x8050		// GL_RGB5
#define RGB8						0x8051		// GL_RGB8
#define RGB10						0x8052		// GL_RGB10
#define RGB12						0x8053		// GL_RGB12
#define RGB16						0x8054		// GL_RGB16

#define RGBA						0x1908		// GL_RGBA
#define RGBA2						0x8055		// GL_RGBA2
#define RGBA4						0x8056		// GL_RGBA4
#define RGBA8						0x8058		// GL_RGBA8
#define RGBA12						0x805A		// GL_RGBA12
#define RGBA16						0x805B		// GL_RGBA16

#define DEPTH_ATTACHMENT			0x8D00		// GL_DEPTH_ATTACHMENT
#define DEPTH_COMPONENT				0x1902		// GL_DEPTH_COMPONENT
#define DEPTH_COMPONENT16			0x81A5		// GL_DEPTH_COMPONENT16
#define DEPTH_COMPONENT24			0x81A6		// GL_DEPTH_COMPONENT24
#define DEPTH_COMPONENT32			0x81A7		// GL_DEPTH_COMPONENT32
#define DEPTH_COMPONENT32F			0x8CAC		// GL_DEPTH_COMPONENT32F

#define DEPTH_STENCIL_ATTACHMENT	0x821A		// GL_DEPTH_STENCIL_ATTACHMENT
#define DEPTH_STENCIL				0x84F9		// GL_DEPTH_STENCIL
#define DEPTH24_STENCIL8			0x88F0		// GL_DEPTH24_STENCIL8
#define DEPTH32F_STENCIL8			0x8CAD		// GL_DEPTH32F_STENCIL8

// TODO: make more formats

namespace HK {
	struct TextureProps {
		uint16_t textureMinFilter;
		uint16_t textureMagFilter;

		uint16_t textureWrapS;
		uint16_t textureWrapT;

		TextureProps()
			: textureMinFilter(LINEAR_FILTER), textureMagFilter(LINEAR_FILTER), textureWrapS(REPEAT_WRAP), textureWrapT(REPEAT_WRAP) {}
		TextureProps(const uint16_t _textureMinFilter, const uint16_t _textureMagFilter, const uint16_t _textureWrapS, const uint16_t _textureWrapT)
			: textureMinFilter(_textureMinFilter), textureMagFilter(_textureMagFilter), textureWrapS(_textureWrapS), textureWrapT(_textureWrapT) {}
	};
}

namespace HKCR {
	// Rename to "TextureManager"
	class TextureManager {
	public:
		static const HK::Texture createFromFile(const std::string& pathToTexture, int* width, int* height, const HK::TextureProps& props, const bool flipY, const bool isRectanglePrimitive);
		static const HK::Texture create(unsigned int width, unsigned int height, const HK::TextureProps& props, const bool isRectanglePrimitive, uint8_t channels, const unsigned int imageFormat, const unsigned int dataFormat, const void* textureData);
	private:
		static const uint64_t getTextureHandle(const unsigned int textureID);
		static const uint64_t createTextureHandle(const unsigned int textureID);
		static void removeTextureHandle(const unsigned int textureID);

		/*static void pullHandlesToGPU(const void* handles);
		static uint16_t getHandleIndexIntoGPU(const uint64_t handle);*/

		static void makeHandleResident(const uint64_t handle);
		static void makeHandleNonResident(const uint64_t handle);
		static void makeAllHandelesNonResident();

		static std::unordered_set<GLuint64> m_textureHandles;

		friend class App;
	};
}
