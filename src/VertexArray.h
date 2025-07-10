#pragma once
#include "VertexBuffer.h"

class VertexArray {
	private:
		unsigned int mRendererID;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind();
		void Unbind();
};
