#pragma once

#include "AutoPtr.h"
#include "GraphicsDriverAPI.h"

namespace LindaEngine
{
	struct RenderState;

	class RenderAPIContext
	{
	public:
		virtual void SetViewport(int xStart, int yStart, int width, int height) = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear(bool color, bool depth, bool stencil) = 0;

		virtual void CreateVertexArrays(int count, unsigned int VAO) = 0;
		virtual void DeleteVertexArrays(int count, unsigned int VAO) = 0;
		virtual void BindVertexArray(unsigned int VAO) = 0;

		virtual void CreateIndexBuffer(unsigned int IBO, int count, void* data, bool is32Bit, bool isStatic) = 0;
		virtual void CreateVertexBuffer(unsigned int VBO, int size, void* data, bool isStatic) = 0;
		virtual void UpdateVertexData(int offset, int size, void* data) = 0;
		virtual void BindBuffer(unsigned int target, unsigned int buffer) = 0;
		virtual void UnBindBuffer(unsigned int target) = 0;
		virtual void DeleteBuffers(int count, unsigned int id) = 0;

		virtual void CheckRenderState(RenderState& state, RenderState& materialState) = 0;

		static Scope<RenderAPIContext> Create();
	};
}
