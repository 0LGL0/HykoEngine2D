#pragma once

#include "Hyko.h"
#include "Platform/GL.h"

using GLuint = unsigned int;
namespace HKCR {
	template<typename T>
	class VAO {
	public:
		VAO() {
			glGenVertexArrays(1, &m_id);
			m_attribsDataType = HKCR::getOpenGLType<T>();
		}

		inline void addNewAttrib(const uint8_t index, const uint8_t size, const uint8_t relativeOffset) const {
			glEnableVertexArrayAttrib(m_id, index);
			glVertexArrayAttribFormat(m_id, index, size, m_attribsDataType, GL_FALSE, sizeof(T) * relativeOffset);
			glVertexArrayAttribBinding(m_id, index, 0);
		}

		inline void linkVBO(const uint8_t vboID, const uint8_t allAttribsSize) const {
			glVertexArrayVertexBuffer(m_id, 0, vboID, 0, allAttribsSize * sizeof(T));
		}

		inline void linkEBO(const uint8_t eboID) const {
			glVertexArrayElementBuffer(m_id, eboID);
		}

		const inline GLuint getID() { return m_id; }

		void bind() const {
			glBindVertexArray(m_id);
		}

		void unbind() const {
			glBindVertexArray(0);
		}
	private:
		GLuint m_id;

		uint16_t m_attribsDataType;
	};
}