#pragma once

#include "Hyko.h"

using GLuint = unsigned int;

namespace HKCR {
	class RBO {
	public:
		RBO(const uint32_t width, const uint32_t height);
		~RBO();
		
		void fillBuffer(const unsigned int format) const;
		void resize(const uint32_t newWidth, const uint32_t newHeight, const unsigned int newFormat);

		void bind() const;
		void unbind() const;

		const GLuint getRBO() const;

	private:
		void init(const uint32_t width, const uint32_t height);
		void deleteRBO();

		GLuint m_id;

		uint32_t m_width;
		uint32_t m_height;
	};
}