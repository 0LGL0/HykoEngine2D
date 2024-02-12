#pragma once

#include "Hyko.h"

#include "Events/Events.h"

#include <spdlog/logger.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HKCR {
	struct WindowProps {
		uint32_t Width;
		uint32_t Height;
		uint16_t MaxFPS;

		GLFWwindow* Window;

		std::shared_ptr<Events> engineEvents = std::make_shared<Events>();

		std::string Title;

		bool isVSync = false;

		WindowProps(const uint32_t& _width = 1920, const uint32_t& _height = 1080, const uint16_t& _maxFPS = 60, const std::string& _title = "HykoGame", GLFWwindow* _window = nullptr)
			: Width(_width), Height(_height), MaxFPS(_maxFPS), Title(_title), Window(_window) {}

		WindowProps& operator=(WindowProps&& other) {
			Width = std::move(other.Width);
			Height = std::move(other.Height);
			MaxFPS = std::move(other.MaxFPS);
			Title = std::move(other.Title);
			Window = std::move(other.Window);

			return *this;
		}
	};

	class GameWindow {
	public:
		GameWindow() = default;
		GameWindow(const GameWindow&) = delete;
		~GameWindow() = default;

		GameWindow& operator=(const GameWindow&) = delete;

		void init(WindowProps& props);

		// getters
		const uint32_t getWidth() const;
		const uint32_t getHeight() const;
		const uint32_t getMaxFPS() const;
		GLFWwindow* getNativeWindow() const;
		WindowProps& getProps();
		static GameWindow* getInstance();
		const std::shared_ptr<Events> getEvents() const;

		// setters
		void setWidth(const uint32_t& width);
		void setHeight(const uint32_t& height);
		void setTitle(const std::string& title);
		void setMaxFPS(const uint16_t& value);

		void VSync(bool value = true); // enable VSync
		const bool isVSync() const; // VSync is enabled?
	private:
		WindowProps m_props;

		static GameWindow* m_instance;

		friend class App;
	};
}
