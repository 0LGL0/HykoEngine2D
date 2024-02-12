#pragma once

#include "Hyko.h"
#include "Tools/Log/Logger.h"

#include <glad/glad.h>

using GLuint = unsigned int;

namespace HKCR {
	template<typename T>
	class SSBO {
	public:
		SSBO() {
			glCreateBuffers(1, &m_ssbo);
		}
		~SSBO() {
			if (m_ssbo)
				glDeleteBuffers(1, &m_ssbo);
		}

		inline void bind() const {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
		}

		inline void unbind() const {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		inline void allocateMem(const uint16_t numOfClusters, const uint8_t bindIndx) {
			m_bindIndx = bindIndx;
			m_clusterSize = numOfClusters * sizeof(T);

			glNamedBufferData(m_ssbo, m_clusterSize, nullptr, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindIndx, m_ssbo);
		}

		inline void addData(const T* data, const size_t dataSize) {
			static uint8_t numOfClusters = 1;
			// TODO: Сделать новую функцию для изменения данных по определенному офсету
			if (m_lastBufferDataSize + dataSize > m_clusterSize * numOfClusters) {
				const uint8_t numOfNewClusters = dataSize / static_cast<float>(m_clusterSize);

				m_ssbo = resizeBuffer(m_ssbo, m_lastBufferDataSize, m_clusterSize * (numOfNewClusters + numOfClusters));
				numOfClusters += numOfNewClusters; // Add new clusters
			}
			glNamedBufferSubData(m_ssbo, m_lastBufferDataSize, dataSize, data);

			m_cpuSSBO.assign(data, data + (dataSize / sizeof(T)));
			m_lastBufferDataSize += dataSize;
		}

		inline void changeData(const T* oldData, const T* newData, const size_t dataSize) {
			glNamedBufferSubData(m_ssbo, findOffset<T>(oldData), dataSize, newData);
		}

		inline const uint16_t findIndex(const T* data) const {
			const auto it = std::find(m_cpuSSBO.begin(), m_cpuSSBO.end(), data);
			if (it == m_cpuSSBO.end()) {
				HK_LOG_ERROR("there is no element associated with this data ({})", data);
				abort();
			}
			return std::distance(m_cpuSSBO.begin(), it);
		}

		inline const uint16_t findOffset(const T* data) const { // Find the offset of an element in SSBO by the entity ID
			return findIndex(data) * sizeof(T);
		}
	private:
		inline const GLuint resizeBuffer(const GLuint readBuffer, const size_t readBSize, const size_t newSize) {
			GLuint newBuffer;
			glCreateBuffers(1, &newBuffer);

			glNamedBufferData(newBuffer, newSize, nullptr, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindIndx, newBuffer);

			glCopyNamedBufferSubData(readBuffer, newBuffer, 0, 0, readBSize);
			glDeleteBuffers(1, &readBuffer); // We don't need the old buffer anymore

			return newBuffer;
		}

		uint8_t m_bindIndx = 0;
		size_t m_clusterSize = 0;
		uint32_t m_lastBufferDataSize = 0;

		//				data
		std::vector<T> m_cpuSSBO;

		GLuint m_ssbo;
	};
}