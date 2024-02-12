#pragma once

#include "Hyko.h"

#include <glm/glm.hpp>

namespace HK {
	struct UV {
		std::vector<glm::vec2> uvCoords;

		UV()
			: uvCoords(
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(1.0f, 0.0f),
					glm::vec2(1.0f, 1.0f),
					glm::vec2(0.0f, 1.0f)
				}) {}
		UV(const bool isRectangle, const float textureWidth, const float textureHeight) {
			normalize(isRectangle, textureWidth, textureHeight);
		}
		UV(const std::vector<glm::vec2>& _uvCoords)
			: uvCoords(_uvCoords) {}

		inline void normalize(const bool isRectangle, const float textureWidth, const float textureHeight) {
			float normalizedWidth = 1.0f;
			float normalizedHeight = 1.0f;

			//	if (textureWidth > textureHeight)
					//normalizedHeight /= (textureHeight / textureWidth);
				//else if(textureWidth < textureHeight)
					//normalizedWidth /= (textureWidth / textureHeight);

			if (isRectangle) {
				uvCoords =
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(normalizedWidth, 0.0f),
					glm::vec2(normalizedWidth, normalizedHeight),
					glm::vec2(0.0f, normalizedHeight)
				};
			}
			else {
				uvCoords =
				{
					glm::vec2(0.0f, 0.0f),
					glm::vec2(normalizedWidth, 0.0f),
					glm::vec2(normalizedWidth / 2.0f, normalizedHeight),
				};
			}
		}
	};
}