#include "Renderer.h"
#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// Returns `true` when no error is detected.
// Returns `false` otherwise.
bool GLLogCall(const char* funcName, const char* filename, int line) {
	while (GLenum error = glGetError()) {
		std::cout << filename << ":" << line << " - OpenGL ERROR (" << error << "): from " << funcName << std::endl;
		return false;
	}
	return true;
}
