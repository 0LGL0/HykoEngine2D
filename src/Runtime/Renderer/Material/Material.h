#pragma once

#include "Hyko.h"
#include "Renderer/Texture/Texture.h"

#include <glm/glm.hpp>

namespace HK {
	struct Material {
		Texture texture;
		glm::vec3 color;

		Material()
			: texture(Texture()), color(glm::vec3(1.0f, 1.0f, 1.0f)) {}
		Material(const Texture& _texture, const glm::vec3& _color)
			: texture(_texture), color(_color) {}
	};
}