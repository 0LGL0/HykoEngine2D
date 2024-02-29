#pragma once

#include "Hyko.h"

#include "Window/GameWindow.h"
#include "Scene/SceneManager.h"
#include "Platform/Buffers/GL/FBO/FBO.h"
#include "Platform/ShaderTools/Shader.h"

namespace HKCR {
	class App {
	public:
		App();
		~App();
	private:
		void onWindowResizing(const float newWidth, const float newHeight);
		uint16_t m_cX = 0;
		uint16_t m_cY = 0;
		uint32_t m_cWW = INITIAL_GAME_WINDOW_WIDTH;
		uint32_t m_cWH = INITIAL_GAME_WINDOW_HEIGHT;


		GameWindow* m_gameWindow;

		SceneManager* m_sceneManager;
		Shader m_fboShader;
	};
}