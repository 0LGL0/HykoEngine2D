cmake_minimum_required(VERSION 3.20)

project(Hyko_Engine
		VERSION 1.0
		LANGUAGES CXX
)

if (MSVC)
	set_target_properties(${BUILD_TARGET} PROPERTIES LINK_FLAGS "/PROFILE")
endif()

# USE C++17
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(EnTT CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Runtime)

string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
if(build_type STREQUAL debug)
	add_compile_definitions(__DEBUG)
endif()

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/Runtime/*.h)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Runtime)

add_executable(${PROJECT_NAME} "src/GameMain.cpp" ${HEADERS})

target_link_libraries(${PROJECT_NAME}
	PRIVATE spdlog::spdlog_header_only

	PUBLIC HykoEngine
)