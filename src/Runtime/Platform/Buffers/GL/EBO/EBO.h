#pragma once

using GLuint = unsigned int;

namespace HKCR {
	class EBO {
	public:
		EBO();
		void fillBuffer(const size_t dataSize, const void* data) const;

		const inline GLuint getID() { return m_id; }

		void bind() const;
		void unbind() const;
	private:
		GLuint m_id;
	};
}