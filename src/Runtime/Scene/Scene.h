#pragma once

#include "Hyko.h"
#include "Core/UUID/UUID.h"
#include "Events/Events.h"
#include "Window/GameWindow.h"

#include <spdlog/logger.h>

#include <entt/entity/registry.hpp>

namespace HK {
	class Entity;
}

namespace HKCR {
	class Renderer;

	class Scene {
	public:
		Scene();
		~Scene();

		HK::Entity& createEntity();
		const bool removeEntity(HK::Entity& entity);

		entt::registry& getReg();
		void setReg(entt::registry& reg);
		void lastPreparation();
		void updateScene();

		void setCurrentCamera(HK::Entity& camera);
		std::shared_ptr<HK::Entity> getCurrentCamera() const;
	private:
		entt::registry m_reg;

		Renderer* m_Renderer;

		std::shared_ptr<HK::Entity> m_currentCamera = std::make_shared<HK::Entity>();
		std::shared_ptr<Events> m_engineEventsPtr;
		GameWindow* m_gameWindowPtr = nullptr;

		friend HK::Entity;
	};
}