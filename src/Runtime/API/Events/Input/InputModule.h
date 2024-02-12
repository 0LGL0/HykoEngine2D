#pragma once

#include "Core/Input.h"

#include <glm/glm.hpp>

namespace HKCR {
	class GameWindow;
	struct InputProps {
		HKCR::Key pressedKey;
		HKCR::Action keyAction;
		HKCR::MouseButton pressedMouseButton;
		HKCR::Action mouseButtonAction;

		double cursorPosX = 0.0;
		double cursorPosY = 0.0;

		double scrollX = 0.0;
		double scrollY = 0.0;
	};

	class Input {
	public:
		Input() = default;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		~Input() = default;

		[[nodiscard]]
		static const bool isKey(const HKCR::Key& key, const HKCR::Action& action, const HKCR::KeyMod& mod);
		[[nodiscard]]
		static const bool isMouseButton(const HKCR::MouseButton& mouseButton, const HKCR::Action& action, const HKCR::KeyMod& mod);
		[[nodiscard]]
		static const glm::mediump_dvec2 cursorPos();
		[[nodiscard]]
		static const glm::mediump_dvec2 scroll();

		// TODO: Make it private
	private:
		friend HKCR::GameWindow;

		static void updateKey(const HKCR::Key key, const HKCR::Action action);
		static void updateMouseButton(const HKCR::MouseButton mouseButton, const HKCR::Action action);
		static void updateCursorPos(const double x, const double y);
		static void updateScroll(const double x, const double y);

		static bool m_enabled;
		static HKCR::InputProps m_props;
	};
}