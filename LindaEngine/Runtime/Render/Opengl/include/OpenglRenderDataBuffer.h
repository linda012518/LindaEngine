#pragma once

#include "RenderDataBuffer.h"

namespace LindaEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(void* data, uint32_t size) override;

	private:
		int _rendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return _count; }

	private:
		int _rendererID;
		uint32_t _count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const { return _vertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }

	private:
		int _rendererID;
		Ref<VertexBuffer> _vertexBuffers;
		Ref<IndexBuffer> _indexBuffer;
	};
}
