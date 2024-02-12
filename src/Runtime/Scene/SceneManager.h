#pragma once

#include "Scene.h"

namespace HKCR {
	class SceneManager {
	public:
		~SceneManager();

		void setCurrentScene(Scene* scene);
		Scene* getCurrentScene() const;

		static SceneManager* getInstance();
	private:
		Scene* m_currentScene = nullptr;
	};
}