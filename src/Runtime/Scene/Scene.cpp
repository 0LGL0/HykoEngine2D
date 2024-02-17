#include "Scene.h"

#include "Entity/Components/TransformComponent.h"
#include "Entity/Components/SpriteComponent.h"
#include "Entity/Components/IDComponent.h"
#include "Entity/Components/CameraComponent.h"

#include "Tools/Log/Logger.h"
#include "Renderer/Renderer.h"

#include "Scene/Entity/Entity.h"

#include <glad/glad.h>

namespace HKCR {
	Scene::Scene() {
		m_Renderer = new Renderer;
		m_Renderer->init();

		m_gameWindowPtr = HKCR::GameWindow::getInstance();
		m_engineEventsPtr = m_gameWindowPtr->getEvents();
	}

	Scene::~Scene()
	{
		delete m_Renderer;
	}

	HK::Entity& Scene::createEntity() {
		auto newEntity = HK::Entity(m_reg.create(), this);

		newEntity.addComponent<HK::LayerComponent>();
		newEntity.addComponent<HK::TransformComponent>();
		const auto& uuid = (uint64_t)newEntity.addComponent<HK::IDComponent>().id;
		HK_LOG_INFO("Created entity with UUID: {}", uuid);

		return newEntity;
	}

	const bool Scene::removeEntity(HK::Entity& entity) {
		const auto& entityDesc = entity.m_entity;
		const auto& uuid = (uint64_t)entity.getComponent<HK::IDComponent>().id;
		if (m_reg.destroy(entityDesc)) {
			if (m_Renderer->removeObject(static_cast<entityID>(entity.m_entity))) {
				HK_LOG_INFO("Entity with UUID: {0} removed", uuid);
				return true;
			}
		}
		return false;
	}

	entt::registry& Scene::getReg() {
		return m_reg;
	}

	void Scene::setReg(entt::registry& reg) {
		auto regData = reg.data();
		m_reg.assign(regData, regData + reg.size(), reg.released());
	}

	void Scene::lastPreparation() {
		m_Renderer->lastPreparation();
	}

	void Scene::updateScene() {
		const auto& group = m_reg.group<HK::SpriteComponent>(entt::get<HK::TransformComponent>);
		for (auto& entityIt = group.rbegin(); entityIt != group.rend(); entityIt++) {
			const auto& entity = *entityIt;

			const auto& [transform, sprite] = m_reg.get<HK::TransformComponent, HK::SpriteComponent>(entity);
			const auto& [layerComp, idComp] = m_reg.get<HK::LayerComponent, HK::IDComponent>(entity);

			const glm::mat3 transformMat = transform.getTransform();
			const entityID entityId = static_cast<entityID>(entity);
			const uint8_t layerIndx = layerComp.layerIndex;

			switch (sprite.primitive) {
			case HK::Primitive::Rectangle:
				m_Renderer->createRectangle(entityId, layerIndx, transformMat, sprite.material, sprite.material.texture.uv);
				break;
			case HK::Primitive::Triangle:
				m_Renderer->createTriangle(entityId, layerIndx, transformMat, sprite.material, sprite.material.texture.uv);
			}
		}

		/*if (m_engineEventsPtr->WindowResizing) {

		}*/

		m_Renderer->renderAll();
	}

	void Scene::setCurrentCamera(HK::Entity& camera)
	{
		if (camera.hasAllComponents<HK::CameraComponent>()) {
			m_currentCamera = std::make_shared<HK::Entity>(camera);

			const float ratio = (float)INITIAL_GAME_WINDOW_WIDTH / INITIAL_GAME_WINDOW_HEIGHT;
			m_currentCamera->getComponent<HK::CameraComponent>().updateProjectionMat(1920.0f, 1080.0f);
		}
		else
			HK_LOG_INFO("Camera (pointer: {}) does not contain a CameraComponent");
	}

	std::shared_ptr<HK::Entity> HKCR::Scene::getCurrentCamera() const
	{
		if (m_currentCamera != nullptr)
			return m_currentCamera;
		else
			HK_LOG_INFO("The current camera was empty");
	}
}