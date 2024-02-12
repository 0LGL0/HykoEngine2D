#pragma once

#include "Hyko.h"
#include "Tools/Log/Logger.h"

#include <entt/entity/registry.hpp>

namespace HKCR {
	class Scene;
}

namespace HK {
	class Entity {
	public:
		Entity() = default;
		Entity(const entt::entity& entity, HKCR::Scene* scene)
			: m_entity(entity), m_scene(scene) {}
		Entity(const Entity& other) {
			m_scene = other.m_scene;
			m_entity = other.m_entity;
		}

		Entity& operator=(Entity& other) {
			m_scene = other.m_scene;
			m_entity = other.m_entity;

			return *this;
		}

		template<typename Type, typename... Args>
		inline Type& addComponent(Args&&... args) const {
			if (hasAllComponents<Type>()) {
				HK_LOG_WARNING("This entity ({}) already has a {}", static_cast<entityID>(m_entity), typeid(Type).name());
			}
			else {
				return m_scene->getReg().emplace<Type>(m_entity, std::forward<Args>(args)...);
			}
		}

		template<typename Type>
		[[nodiscard]]
		inline decltype(auto) getComponent() const {
			if (hasAllComponents<Type>()) {
				return m_scene->getReg().get<Type>(m_entity);
			}
			else
				HK_LOG_WARNING("This object ({}) does not contain these components", static_cast<entityID>(m_entity));
		}

		/*
		 * @brief Removes the given components from an entity
		 */
		template<typename Type, typename ...Other>
		inline void remove() const {
			m_scene->getReg().remove<Type, Other...>(m_entity);
		}

		/*
		 * @brief Removes the given components from all the entities in a range
		 */
		template<typename Type, typename ...Other, typename It>
		inline void remove(const It& itBegin, const It& itEnd) const {
			m_scene->getReg().remove<Type, Other...>(itBegin, itEnd);
		}

		template<typename Type>
		[[nodiscard]]
		inline const bool hasAllComponents() const {
			return m_scene->getReg().all_of<Type>(m_entity);
		}

		template<typename Type>
		[[nodiscard]]
		inline const bool hasAnyComponents() const {
			return m_scene->getReg().any_of<Type>(m_entity);
		}

		///*
		// * @brief Get the entity descriptor
		// */
		//inline entt::entity& get() {
		//	return m_entity;
		//}
	private:
		friend HKCR::Scene;

		HKCR::Scene* m_scene = nullptr;
		entt::entity m_entity;
	};
}