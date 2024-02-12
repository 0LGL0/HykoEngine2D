#pragma once

namespace HKCR {
	struct Events {
		bool WindowResizing = false;
		bool WindowMoving = false;
		bool CursorMoving = false;
	private:
		void reset() {
			WindowMoving = false;
			WindowResizing = false;
			CursorMoving = false;
		}

		friend class App;
	};
}