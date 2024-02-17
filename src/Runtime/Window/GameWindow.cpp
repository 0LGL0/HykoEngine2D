#include "GameWindow.h"

#include "Tools/Log/Logger.h"
#include "Events/Input/InputModule.h"

#define LOGGER_NAME "GameWindowLogger"

namespace HKCR {
	GameWindow* GameWindow::m_instance = nullptr;

	void GameWindow::init(HKCR::WindowProps& props) {
		m_props = std::move(props);

		if (!glfwInit()) {
			HK_LOG_ERROR("GLFW was not initialized");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_props.Window = glfwCreateWindow(m_props.Width, m_props.Height, m_props.Title.c_str(), nullptr, nullptr);
		HK_ASSERT(m_props.Window, "GLFW window was not created");

		glfwMakeContextCurrent(m_props.Window);
		glfwSetWindowUserPointer(m_props.Window, &m_props);

		glfwSetWindowPosCallback(m_props.Window, [](GLFWwindow* window, int xpos, int ypos) {
			auto pointer = (HKCR::WindowProps*)glfwGetWindowUserPointer(window);
			pointer->engineEvents->WindowMoving = true;
			});

		glfwSetWindowSizeCallback(m_props.Window, [](GLFWwindow* window, int width, int height) {
			auto pointer = (HKCR::WindowProps*)glfwGetWindowUserPointer(window);
			pointer->Width = width;
			pointer->Height = height;
			pointer->engineEvents->WindowResizing = true;
			});

		glfwSetKeyCallback(m_props.Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto pointer = (HKCR::WindowProps*)glfwGetWindowUserPointer(window);

			//printf("%i, %i\n", key, action);
			Input::updateKey(static_cast<HKCR::Key>(key), static_cast<HKCR::Action>(action));
			});

		glfwSetMouseButtonCallback(m_props.Window, [](GLFWwindow* window, int button, int action, int mods) {
			auto pointer = (HKCR::WindowProps*)glfwGetWindowUserPointer(window);
			Input::updateMouseButton(static_cast<HKCR::MouseButton>(button), static_cast<HKCR::Action>(action));
			});

		glfwSetCursorPosCallback(m_props.Window, [](GLFWwindow* window, double xpos, double ypos) {
			auto pointer = (HKCR::WindowProps*)glfwGetWindowUserPointer(window);
			Input::updateCursorPos(xpos, ypos);

			pointer->engineEvents->CursorMoving = true;
			});

		glfwSetScrollCallback(m_props.Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			auto pointer = (HKCR::WindowProps*)glfwGetWindowUserPointer(window);
			Input::updateScroll(xoffset, yoffset);
			});

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			HK_LOG_ERROR("OpenGL functions have not been loaded");
		}
	}

	GameWindow* GameWindow::getInstance()
	{
		static GameWindow* instance = new GameWindow;
		m_instance = instance;
		return m_instance;
	}

	const std::shared_ptr<Events> GameWindow::getEvents() const
	{
		return m_props.engineEvents;
	}

	GLFWwindow* GameWindow::getNativeWindow() const {
		return m_props.Window;
	}

	const uint32_t GameWindow::getWidth() const {
		return m_props.Width;
	}

	const uint32_t GameWindow::getHeight() const {
		return m_props.Height;
	}

	const uint32_t GameWindow::getMaxFPS() const {
		return m_props.MaxFPS;
	}

	void GameWindow::setWidth(const uint32_t& width) {
		HK_LOG_INFO("GLFW Game window width updated: {}", width);
		m_props.Width = width;
	}

	void GameWindow::setHeight(const uint32_t& height) {
		HK_LOG_INFO("GLFW Game window height updated: {}", height);
		m_props.Height = height;
	}

	void GameWindow::setTitle(const std::string& title) {
		HK_LOG_INFO("GLFW Game window title updated: {}", title);
		m_props.Title = title;

		glfwSetWindowTitle(m_props.Window, title.c_str());
	}

	void GameWindow::setMaxFPS(const uint16_t& value) {
		HK_LOG_INFO("GLFW Game window max fps updated: {}", value);
		m_props.MaxFPS = value;
	}

	void GameWindow::VSync(bool value) {
		HK_LOG_INFO("GLFW Game window VSync value updated: {}", value);
		m_props.isVSync = value;
		glfwSwapInterval(value);
	}

	const bool GameWindow::isVSync() const {
		return m_props.isVSync;
	}

	HKCR::WindowProps& GameWindow::getProps()
	{
		return m_props;
	}
}
