cmake_minimum_required(VERSION 3.20)

project(Hyko_Runtime 
	LANGUAGES CXX
)

# USE C++17
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(HykoEngine STATIC
	Core/Application.cpp							# Application
	Platform/Buffers/GL/FBO/FBO.cpp					# FBO
	Window/GameWindow.cpp							# GameWindow
	Platform/GL.cpp									# GL
	Events/Input/InputModule.cpp					# InputModule
	Platform/Buffers/GL/VBO/VBO.cpp					# VBO
	Platform/Buffers/GL/EBO/EBO.cpp					# EBO
	Platform/Buffers/GL/VAO/VAO.cpp					# VAO
	Platform/Buffers/GL/SSBO/SSBO.cpp				# SSBO
	Renderer/Texture/TextureManager.cpp				# TextureManager
	Renderer/Renderer.cpp							# Renderer
	Scene/Scene.cpp									# Scene
	Scene/SceneManager.cpp							# SceneManager
	Tools/Sys.cpp									# Sys
	Platform/ShaderTools/Shader.cpp					# Shader
	Core/UUID/UUID.cpp								# UUID
	Tools/Log/Logger.cpp							# Logger

	Core/Application.h								# Application
	Scene/Entity/Entity.h							# Entity
	Platform/Buffers/GL/FBO/FBO.h					# FBO
	Window/GameWindow.h								# GameWindow
	Platform/GL.h									# GL
	Platform/ShaderTools/Shader.h					# Shader
	Tools/Sys.h										# Sys
	Scene/Scene.h									# Scene
	Scene/SceneManager.h							# SceneManager
	Scene/Entity/Components/SpriteComponent.h		# SpriteComponent
	Scene/Entity/Components/TransformComponent.h	# TransformComponent
	Scene/Entity/Components/CameraComponent.h		# CameraComponent
	Scene/Entity/Components/IDComponent.h			# IDComponent
	Renderer/Renderer.h								# Renderer
	Renderer/UV/UV.h								# UV
	Renderer/Texture/TextureManager.h				# TextureManager
	Renderer/Texture/Texture.h						# Texture
	Renderer/Material/Material.h					# Material
	Platform/Buffers/GL/VBO/VBO.h					# VBO
	Platform/Buffers/GL/EBO/EBO.h					# EBO
	Platform/Buffers/GL/VAO/VAO.h					# VAO
	Platform/Buffers/GL/SSBO/SSBO.h					# SSBO
	Math/Matrix/MatrixTransform.h					# MatrixTransform
	Events/Input/InputModule.h						# InputModule
	Events/Events.h									# EngineEvents
	Core/UUID/UUID.h								# UUID
	Core/Input.h									# Input codes
	Tools/Log/Logger.h								# Logger
	Scene/Entity/Components/LayerComponent.h		# LayerComponent
)

target_link_libraries(HykoEngine
	PRIVATE
		OpenGL::GL
		glad::glad
		glfw
		EnTT::EnTT
		spdlog::spdlog_header_only
		fmt::fmt
		glm::glm
)