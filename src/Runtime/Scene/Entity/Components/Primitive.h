#pragma once

#include "Hyko.h"
#include "glm/glm.hpp"

namespace HKCR {
	struct PrimitiveComponent {
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		uint16_t Segments = 0; // Only for the circle
	};
}