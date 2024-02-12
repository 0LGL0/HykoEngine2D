#pragma once

#include "Core/UUID/UUID.h"

namespace HK {
	/*enum class EntityType {
		Primitive, Camera
	};*/

	struct IDComponent {
		HKCR::UUID id;
		//EntityType entityType;

		IDComponent()
			: id(HKCR::UUID()) {}
		IDComponent(const IDComponent&) = delete;
		IDComponent& operator=(const IDComponent&) = delete;
	};
}