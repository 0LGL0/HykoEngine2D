#pragma once

#include "Hyko.h"

#include "glm/glm.hpp"

namespace HK {
	class MatTransform {
	public:
		static const inline glm::mat3 translate(const glm::vec2& pos) {
			glm::mat3 m = glm::mat3(1.0f);
			m[2] = glm::vec3(pos, 0.0f);

			return m;
		}

		static const inline glm::mat3 scale(const glm::vec2& scale) {
			const glm::mat3 m = glm::mat3(
				scale.x, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f,
				0.0f, 0.0f, 1.0f
			);

			return m;
		}

		static const inline glm::mat3 rot(const float angle) {
			const glm::mat3 m = glm::mat3(
				cos(angle), -sin(angle), 0.0f,
				sin(angle), cos(angle), 0.0f,
				0.0f, 0.0f, 1.0f
			);

			return m;
		}
	};
}
