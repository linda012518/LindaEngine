#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	struct RenderState;
	class RenderAPIContext;

	class Graphic
	{
	public:
		static void SetViewport(int xStart, int yStart, int width, int height);
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear(bool color, bool depth, bool stencil);

		static void CreateVertexArrays(int count, unsigned int VAO);
		static void DeleteVertexArrays(int count, unsigned int VAO);
		static void BindVertexArray(unsigned int VAO);

		static void CreateIndexBuffer(unsigned int IBO, int count, void* data, bool is32Bit = true, bool isStatic = true);
		static void CreateVertexBuffer(unsigned int VBO, int size, void* data, bool isStatic = true);
		static void UpdateVertexData(int offset, int size, void* data);
		static void BindBuffer(unsigned int target, unsigned int buffer);
		static void UnBindBuffer(unsigned int target);
		static void DeleteBuffers(int count, unsigned int buffer);

		static void CheckRenderState(RenderState& state, RenderState& materialState);

	private:
		static Scope<RenderAPIContext> _renderContext;
	};
}
