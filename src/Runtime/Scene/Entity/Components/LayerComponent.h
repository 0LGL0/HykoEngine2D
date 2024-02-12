#pragma once

#include "Hyko.h"

namespace HK {
	struct LayerComponent {
		LayerComponent() = default;
		LayerComponent(const uint8_t index)
			: layerIndex(index) {}

		uint8_t layerIndex = 0;
	};
}