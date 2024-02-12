#include "InputModule.h"

#include "Tools/Sys.h"
#include "Window/GameWindow.h"

namespace HKCR {
	bool Input::m_enabled = false;
	InputProps Input::m_props;

	const bool Input::isKey(const HKCR::Key& key, const HKCR::Action& action, const HKCR::KeyMod& mod)
	{
		if (m_enabled) {
			m_enabled = false;

			if (m_props.pressedKey == key && m_props.keyAction == action)
				return true;
		}

		return false;
	}

	const bool Input::isMouseButton(const HKCR::MouseButton& mouseButton, const HKCR::Action& action, const HKCR::KeyMod& mod)
	{
		if (m_enabled) {
			m_enabled = false;

			if (m_props.pressedMouseButton == mouseButton && m_props.mouseButtonAction == action)
				return true;
		}

		return false;
	}

	const glm::mediump_dvec2 Input::cursorPos() {
		return { m_props.cursorPosX, m_props.cursorPosY };
	}

	const glm::mediump_dvec2 Input::scroll() {
		return { m_props.scrollX, m_props.scrollY };
	}

	void Input::updateKey(const HKCR::Key key, const HKCR::Action action) {
		m_props.pressedKey = key;
		m_props.keyAction = action;
		m_enabled = true;
	}

	void Input::updateMouseButton(const HKCR::MouseButton mouseButton, const HKCR::Action action) {
		m_props.pressedMouseButton = mouseButton;
		m_props.mouseButtonAction = action;
		m_enabled = true;
	}

	void Input::updateCursorPos(const double x, const double y) {
		m_props.cursorPosX = x;
		m_props.cursorPosY = y;
	}

	void Input::updateScroll(const double x, const double y) {
		m_props.scrollX = x;
		m_props.scrollY = y;
	}
}