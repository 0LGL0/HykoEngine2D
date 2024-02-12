#pragma once

#include "Window/GameWindow.h"

#include "Platform/Tools/DLL/DLLLink.h"

namespace HK {
	class Window {
	public:
		Window() {
			m_IWindow = HKCR::loadDLLFunc<HKCR::IGameWindow*>("src\\Runtime\\GameWindow", "_createGameWindow")();
			m_IWindow->setup_logger({ HKCR::loggerSink });
		}
		~Window() {
			HKCR::freeDLL("GameWindow");
		}

		HKCR::IGameWindow* operator->() {
			return m_IWindow;
		}
	private:
		HKCR::IGameWindow *m_IWindow;
	};
}