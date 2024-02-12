#include "SceneManager.h"

namespace HKCR {
	SceneManager::~SceneManager()
	{
		delete m_currentScene;
	}

	void SceneManager::setCurrentScene(Scene* scene)
	{
		m_currentScene = scene;
	}

	Scene* SceneManager::getCurrentScene() const
	{
		return m_currentScene;
	}

	SceneManager* SceneManager::getInstance()
	{
		static SceneManager* instance = new SceneManager;
		return instance;
	}
}
