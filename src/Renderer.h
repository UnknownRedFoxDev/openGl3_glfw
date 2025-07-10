#pragma once

// :s/^\([^aA-zZ]*\)\([^;]*\)/\1GLCall(\2) my beloved
#include <GL/glew.h>
#include <cstdlib>

#define ASSERT(x) if (!(x)) exit(-1)
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#define DEBUG 1

typedef unsigned int uint;

void GLClearError();

// Returns `true` when no error is detected.
// Returns `false` otherwise.
bool GLLogCall(const char* funcName, const char* filename, int line);
