#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <typeinfo>
#include <filesystem>
#include <functional>
#include <forward_list>
#include <set>

#include <Windows.h>

#include <glm/glm.hpp>

#include "Math/Matrix/MatrixTransform.h"

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#define INITIAL_GAME_WINDOW_WIDTH 1280
#define INITIAL_GAME_WINDOW_HEIGHT 720

using entityID = uint32_t;