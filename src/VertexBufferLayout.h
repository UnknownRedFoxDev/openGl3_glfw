#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
	unsigned int count;
	unsigned int GLType;
	bool normalized;
};

class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> mElements;

		unsigned int mStride;
	public:
		VertexBufferLayout() : mStride(0) {
		}

		template<typename T>
		void Push(unsigned int count) { static_assert(false); }

		template<>
		void Push<float>(unsigned int count) {
			mElements.push_back({count, GL_FLOAT, false});
			mStride += sizeof(GLfloat); // Somewhat hardcoded
		}

		template<>
		void Push<unsigned int>(unsigned int count) {
			mElements.push_back({count, GL_UNSIGNED_INT, false});
			mStride += sizeof(GLuint); // Somewhat hardcoded
		}

		template<>
		void Push<unsigned char>(unsigned int count) {
			mElements.push_back({count, GL_UNSIGNED_BYTE, true});
			mStride += sizeof(GLbyte); // Somewhat hardcoded
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return mElements; }
		inline unsigned int GetStride() const { return mStride; }
};
