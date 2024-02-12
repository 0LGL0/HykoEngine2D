#include "GL.h"

void HKCR::setGLLog(const GLFWerrorfun &func) {
	glfwSetErrorCallback(func);
}