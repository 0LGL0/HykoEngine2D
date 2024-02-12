#pragma once

#include "Hyko.h"

#include <glm/gtx/transform.hpp>

using uint32_t = unsigned int;

namespace HKCR {
	class Scene;
	class App;
}

namespace HK {
	struct CameraComponent {
		uint32_t screenWidth = INITIAL_GAME_WINDOW_WIDTH;
		uint32_t screenHeight = INITIAL_GAME_WINDOW_HEIGHT;

		const glm::mat4 getProjectionMatrix() const {
			return m_projectionMat;
		}

		const glm::mat4 getViewMatrix() const {
			return m_viewMat;
		}
	private:
		void updateViewMat(const glm::vec2 newPos) {
			m_viewMat = glm::mat4(HK::MatTransform::translate(newPos));
		}

		void updateProjectionMat(const uint32_t newScreenWidth, const uint32_t newScreenHeight) {
			const float ratio = newScreenWidth / newScreenHeight;

			m_projectionMat = glm::ortho(-(newScreenWidth / 2.0f * ratio), newScreenWidth / 2.0f * ratio, -(newScreenHeight / 2.0f), newScreenHeight / 2.0f);
		}

		glm::mat4 m_projectionMat = glm::mat4(1.0f);

		// TODO: replace it with just the camera position
		glm::mat4 m_viewMat = glm::mat4(1.0f);

		friend HKCR::Scene;
		friend HKCR::App;
	};
}