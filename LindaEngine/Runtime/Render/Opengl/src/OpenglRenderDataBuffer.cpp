#include "OpenglRenderDataBuffer.h"
#include "Graphic.h"

using namespace LindaEngine;

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : _count(count)
{
	_rendererID = -1;
	Graphic::CreateIndexBuffer(_rendererID, _count, indices, true);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	if (_rendererID != -1)
		Graphic::DeleteBuffers(1, _rendererID);
}

void OpenGLIndexBuffer::Bind() const
{
	Graphic::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
}

void OpenGLIndexBuffer::Unbind() const
{
	Graphic::UnBindBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

////////////////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
	_rendererID = -1;
	Graphic::CreateVertexBuffer(_rendererID, 0, nullptr);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
	_rendererID = -1;
	Graphic::CreateVertexBuffer(_rendererID, size, vertices);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	if (_rendererID != -1)
		Graphic::DeleteBuffers(1, _rendererID);
}

void OpenGLVertexBuffer::Bind() const
{
	Graphic::BindBuffer(GL_ARRAY_BUFFER, _rendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
	Graphic::UnBindBuffer(GL_ARRAY_BUFFER);
}

void OpenGLVertexBuffer::SetData(void* data, uint32_t size)
{
	Bind();
	Graphic::UpdateVertexData(0, size, data);
}

////////////////////////////////////////////////////////////////////////////////////////

OpenGLVertexArray::OpenGLVertexArray()
{
	_rendererID = -1;
	Graphic::CreateVertexArrays(1, _rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	Graphic::DeleteVertexArrays(1, _rendererID);
}

void OpenGLVertexArray::Bind() const
{
	Graphic::BindVertexArray(_rendererID);
}

void OpenGLVertexArray::Unbind() const
{
	Graphic::BindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
{
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
{
	Bind();
}
