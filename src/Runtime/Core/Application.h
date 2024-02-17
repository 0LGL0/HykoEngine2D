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
		void onWindowResizing(const float newWidth, const float newHeight);

		HKCR::GameWindow* m_gameWindow;

		SceneManager* m_sceneManager;
	};
}