#include "OpenglRenderDataBuffer.h"
#include "Graphic.h"
#include "Mesh.h"
#include "glad/glad.h"

using namespace LindaEngine;

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count, bool is32Bit, bool isStatic) : _count(count)
{
	_rendererID = 0;

	glGenBuffers(1, &_rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
	uint32_t size = is32Bit ? sizeof(unsigned int) : sizeof(unsigned short);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * size, indices, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	if (_rendererID != 0)
		glDeleteBuffers(1, &_rendererID);
}

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
}

void OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, int vertexStride, bool isStatic)
{
	_count = size / vertexStride;
	_rendererID = 0;
	glGenBuffers(1, &_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, int vertexStride, bool isStatic)
{
	_count = size / vertexStride;
	_rendererID = 0;
	glGenBuffers(1, &_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	if (_rendererID != 0)
		glDeleteBuffers(1, &_rendererID);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetVertexAttribute(std::vector<VertexAttribute>* attrs)
{
	attributes = attrs;
}

const std::vector<VertexAttribute>* LindaEngine::OpenGLVertexBuffer::GetAttributes() const
{
	return attributes;
}

void OpenGLVertexBuffer::SetData(void* data, uint32_t size)
{
	_count = size;
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

////////////////////////////////////////////////////////////////////////////////////////

OpenGLVertexArray::OpenGLVertexArray()
{
	_rendererID = 0;
	glGenVertexArrays(1, &_rendererID);
	glBindVertexArray(_rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &_rendererID);
}

void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(_rendererID);
}

void OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer, int vertexStride)
{
	Bind();
	buffer->Bind();
	for (auto& attr : *buffer->GetAttributes())
	{
		glEnableVertexAttribArray(attr.index);
		glVertexAttribPointer(attr.index, attr.size, GL_FLOAT, attr.normalized ? GL_TRUE : GL_FALSE, vertexStride, (const void*)(intptr_t)attr.offset);
	}
	_vertexBuffer = buffer;
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
{
	Bind();
	buffer->Bind();
	_indexBuffer = buffer;
}

void OpenGLVertexArray::Draw(DrawType drawType)
{
	Bind();

	unsigned int dt = 0;
	switch (drawType)
	{
	case DrawType::POINTS: dt = GL_POINTS; break;
	case DrawType::LINES: dt = GL_LINES; break;
	case DrawType::LINE_LOOP: dt = GL_LINE_LOOP; break;
	case DrawType::LINE_STRIP: dt = GL_LINE_STRIP; break;
	case DrawType::TRIANGLES: dt = GL_TRIANGLES; break;
	case DrawType::TRIANGLE_STRIP: dt = GL_TRIANGLE_STRIP; break;
	case DrawType::TRIANGLE_FAN: dt = GL_TRIANGLE_FAN; break;
	case DrawType::QUADS: dt = GL_QUADS; break;
	}

	if (nullptr != _indexBuffer)
	{
		glDrawElements(dt, _indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(dt, 0, _isEmpty ? 3 : _vertexBuffer->GetCount());
	}
}
