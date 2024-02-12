#pragma once

#include "Hyko.h"
#include "Renderer/Material/Material.h"

#include <glm/glm.hpp>

namespace HK {
	enum class Primitive {
		Triangle, Rectangle
	};

	struct SpriteComponent {
		Material material;
		Primitive primitive;

		SpriteComponent()
			: primitive(Primitive::Rectangle), material(Material()) {}
		SpriteComponent(const Primitive& _primitive, const Material& _material)
			: primitive(_primitive), material(_material) {}
	};
}