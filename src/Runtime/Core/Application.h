#pragma once

#include "Hyko.h"

#include "Window/GameWindow.h"
#include "Scene/SceneManager.h"
#include "Platform/Buffers/GL/FBO/FBO.h"

namespace HKCR {
	class App {
	public:
		App();
		~App();
	private:
		HKCR::GameWindow* m_gameWindow;

		SceneManager* m_sceneManager;
	};
}