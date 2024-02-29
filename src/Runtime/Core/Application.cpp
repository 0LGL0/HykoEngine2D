#include "Application.h"

#include "Hyko.h"
#include "Tools/Log/Logger.h"
#include "Tools/Sys.h"
#include "Platform/GL.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Texture/TextureManager.h"
#include "Scene/Entity/Components/SpriteComponent.h"
#include "Scene/Entity/Components/TransformComponent.h"
#include "Scene/Entity/Components/CameraComponent.h"
#include "Scene/Entity/Entity.h"
#include "Scene/Scene.h"
#include "Events/Input/InputModule.h"
#include "Platform/Buffers/GL/SSBManager.h"
#include "Platform/Buffers/GL/FBO/FBO.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HKCR {
	App::App() {
		m_sceneManager = SceneManager::getInstance();
		m_gameWindow = GameWindow::getInstance();
		Renderer::m_sceneManager = m_sceneManager;

		HKCR::sysInit();
		HKCR::Logger::loggerInit();

		const std::string gameDirPath = HKCR::getGameDirPath();

		HKCR::setGLLog([](int error_code, const char* description) {
			const std::string tmp = "(OpenGL: " + std::to_string(error_code) + ") " + description;
			HK_LOG_ERROR(tmp);
			});

		m_gameWindow->init(HKCR::WindowProps(INITIAL_GAME_WINDOW_WIDTH, INITIAL_GAME_WINDOW_HEIGHT));

		HKCR::Scene scene;

		m_sceneManager->setCurrentScene(&scene);

		static constexpr float FBOVerticesPos[4 * 6]{
			-1.0f, -1.0f,		0.0f, 0.0f,
			 1.0f, -1.0f,		1.0f, 0.0f,
			-1.0f,  1.0f,		0.0f, 1.0f,

			 1.0f, -1.0f,		1.0f, 0.0f,
			 1.0f,  1.0f,		1.0f, 1.0f,
			-1.0f,  1.0f,		0.0f, 1.0f,
		};

		FBO* fbo = m_sceneManager->getCurrentScene()->getSSBManager().getMainFBO();
		fbo->bind();
		const auto fboRGBTexture = fbo->createRGBTexture(4, RGBA8, RGBA);
		fbo->createDepthTexture(DEPTH_COMPONENT24, DEPTH_ATTACHMENT);
		fbo->unbind();

		m_fboShader.loadVertexShader("res\\Shaders\\FBOPostProcess.vert");
		m_fboShader.loadFragmentShader("res\\Shaders\\FBOPostProcess.frag");

		m_fboShader.compileFragmentShader();
		m_fboShader.compileVertexShader();
		m_fboShader.compileShaderProgram();

		VAO<float> fboVAO;
		VBO fboVBO;

		m_fboShader.bind();
		fboVAO.bind();

		fboVBO.fillBuffer(sizeof(FBOVerticesPos), &FBOVerticesPos);

		fboVAO.linkVBO(fboVBO.getID(), 4);
		fboVAO.addNewAttrib(0, 2, 0);
		fboVAO.addNewAttrib(1, 2, 2);

		m_fboShader.setUniform("FBO_RGB", fboRGBTexture.m_handle);
		fboVAO.unbind();
		m_fboShader.unbind();

		auto nWindow = m_gameWindow->getNativeWindow(); // native glfw window

		HKCR::Input inputModule;

		int width;
		int height;

		HK::TextureProps props;
		props.textureMinFilter = NEREAST_FILTER;
		props.textureMagFilter = NEREAST_FILTER;

		const auto texture = HKCR::TextureManager::createFromFile("src\\Runtime\\StandartProjectIcon.jpg", &width, &height, props, true, true);
		const auto texture2 = HKCR::TextureManager::createFromFile("src\\Runtime\\cpp.jpg", &width, &height, props, true, true);
		const auto texture3 = HKCR::TextureManager::createFromFile("src\\Runtime\\tw.png", &width, &height, props, true, true);
		const auto texture4 = HKCR::TextureManager::createFromFile("src\\Runtime\\OIP.tga", &width, &height, props, true, true);
		const auto texture5 = HKCR::TextureManager::createFromFile("src\\Runtime\\Red.png", &width, &height, props, true, true);
		const auto texture6 = HKCR::TextureManager::createFromFile("src\\Runtime\\Blue.png", &width, &height, props, true, true);

		auto entity6 = scene.createEntity();
		entity6.getComponent<HK::TransformComponent>() = HK::TransformComponent(glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 10.0f), 0.0f);
		entity6.addComponent<HK::SpriteComponent>() = HK::SpriteComponent(HK::Primitive::Rectangle, HK::Material(HK::Texture(), glm::vec3(1.0f, 0.0f, 1.0f)));
		entity6.getComponent<HK::LayerComponent>().layerIndex = 1;

		auto entity = scene.createEntity();
		entity.getComponent<HK::TransformComponent>() = HK::TransformComponent(glm::vec2(140.0f, 20.0f), glm::vec2(20.0f, 20.0f), 0.0f);
		entity.addComponent<HK::SpriteComponent>().material = HK::Material(texture, glm::vec3(1.0f, 1.0f, 1.0f));

		auto entity3 = scene.createEntity();
		entity3.getComponent<HK::TransformComponent>() = HK::TransformComponent(glm::vec2(-570.0f, -20.0f), glm::vec2(50.0f, 50.0f), 0.0f);
		entity3.addComponent<HK::SpriteComponent>().material = HK::Material(texture3, glm::vec3(1.0f, 1.0f, 1.0f));
		entity3.getComponent<HK::LayerComponent>().layerIndex = 6;

		auto entity2 = scene.createEntity();
		entity2.getComponent<HK::TransformComponent>() = HK::TransformComponent(glm::vec2(-550.0f, 30.0f), glm::vec2(30.0f, 30.0f), 0.0f);
		entity2.addComponent<HK::SpriteComponent>().material = HK::Material(texture2, glm::vec3(1.0f, 1.0f, 1.0f));

		auto entity4 = scene.createEntity();
		entity4.getComponent<HK::TransformComponent>() = HK::TransformComponent(glm::vec2(-400.0f, 6.0f), glm::vec2(30.0f, 30.0f), 0.0f);
		entity4.addComponent<HK::SpriteComponent>().material = HK::Material(texture4, glm::vec3(1.0f, 1.0f, 1.0f));

		auto entity5 = scene.createEntity();
		entity5.getComponent<HK::TransformComponent>() = HK::TransformComponent(glm::vec2(80.0f, 20.0f), glm::vec2(50.0f, 50.0f), 0.0f);
		entity5.addComponent<HK::SpriteComponent>().material = HK::Material(HK::Texture(), glm::vec3(0.0f, 1.0f, 1.0f));

		auto camera = scene.createEntity();
		camera.addComponent<HK::CameraComponent>();

		scene.setCurrentCamera(camera);

		scene.lastPreparation();

		m_gameWindow->VSync(false);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glDisable(GL_STENCIL_TEST);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		while (!glfwWindowShouldClose(nWindow)) {
			glViewport(0, 0, m_cWW, m_cWH);
			if (m_gameWindow->getEvents()->WindowResizing)
				onWindowResizing(m_gameWindow->getWidth(), m_gameWindow->getHeight());

			fbo->bind();

			glEnable(GL_DEPTH_TEST);

			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

			//glEnable(GL_SCISSOR_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glDisable(GL_SCISSOR_TEST);

			if (HKCR::Input::isKey(HKCR::Key::SPACE, HKCR::Action::PRESS, HKCR::KeyMod::NONE))
				entity3.getComponent<HK::LayerComponent>().layerIndex = 0;

			scene.updateScene();

			// TODO: Maybe replace with something
			m_gameWindow->m_props.engineEvents->reset();

			glViewport(m_cX, m_cY, m_cWW, m_cWH);

			fbo->unbind();

			m_fboShader.bind();
			fboVAO.bind();
			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			fboVAO.unbind();
			m_fboShader.unbind();

			glfwSwapBuffers(nWindow);
			glfwPollEvents();
		}
	}

	App::~App() {
		auto nWindow = m_gameWindow->getNativeWindow(); // native glfw m_gameWindow->

		TextureManager::makeAllHandelesNonResident();

		glfwDestroyWindow(nWindow);
		glfwTerminate();

		spdlog::shutdown();

		delete m_gameWindow;

		// Deleting in the scene destructor
		//delete m_sceneManager;
	}

	// Eeeee... it's veryyy scary)
	void HKCR::App::onWindowResizing(const float newWidth, const float newHeight)
	{
		/*const float w = m_gameWindow->getWidth();
		const float h = m_gameWindow->getHeight();*/

		// For editor window
		//const float aspect = w / h;
		//glViewport(0, 0, w, h);

		///*fbo.setWidth(w);
		//fbo.setHeight(h);*/

		//auto& camComponent = m_sceneManager->getCurrentScene()->getCurrentCamera()->getComponent<HK::CameraComponent>();
		//camComponent.updateProjectionMat(w * aspect, h * aspect);


		// TODO: Add the ability to change the aspect ratio of the window	
		const auto allFBOs = m_sceneManager->getCurrentScene()->getSSBManager().getAllFBOs();
		const auto winMonitor = m_gameWindow->getWindowMonitorInfo().rcMonitor;
		const float mainAspect = (winMonitor.right - winMonitor.left) / (float)(winMonitor.bottom - winMonitor.top);
		m_cWH = newWidth / mainAspect;
		m_cWW = newWidth;

		m_cX = 0;
		m_cY = 0;

		/*if (m_cWH >= newHeight) {
			m_cWW = m_cWH * mainAspect;
			m_cX = newWidth / 2.0f - m_cWW / 2.0f;
			m_cWH = newHeight;
		}
		else m_cY = newHeight / 2.0f - m_cWH / 2.0f;*/

		if (m_cWH > newHeight)
			m_cWH = newHeight;

		m_cWW = m_cWH * mainAspect;

		m_cX = newWidth / 2 - m_cWW / 2;
		m_cY = newHeight / 2 - m_cWH / 2;

		//glViewport(m_cX, m_cY, m_cWW, m_cWH);

		for (const auto fbo : allFBOs)
			fbo->updateSize(m_cWW, m_cWH, DEPTH_COMPONENT24);

		/*m_fboShader.bind();
		m_fboShader.setUniform("FBO_RGB", fbo.getRGBTexture().m_handle);
		m_fboShader.unbind();*/

		//auto& camComponent = m_sceneManager->getCurrentScene()->getCurrentCamera()->getComponent<HK::CameraComponent>();
		//camComponent.updateProjectionMat(m_cWW, m_cWH);

		HK_LOG_INFO("Window was resized (new size: {}x{})", newWidth, newHeight);

		/*glScissor(m_cX, m_cY, m_cWW, m_cWH);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);*/
	}
}