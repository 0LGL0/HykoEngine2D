#pragma once

#include "Math/Matrix/MatrixTransform.h"

#include <glm/glm.hpp>

namespace HK {
	struct TransformComponent {
		glm::vec2 translate = glm::vec2(0.0f);
		glm::vec2 scale = glm::vec2(10.0f);
		float rotate = 0.0f;

		const inline glm::mat3& getTransform() const {
			return HK::MatTransform::translate(translate)
				* HK::MatTransform::rot(rotate)
				* HK::MatTransform::scale(scale);
		}

		TransformComponent() = default;
		TransformComponent(const glm::vec2& _translate, const glm::vec2 _scale, const float _rotate)
			: translate(_translate), scale(_scale), rotate(_rotate) {}
	};
}