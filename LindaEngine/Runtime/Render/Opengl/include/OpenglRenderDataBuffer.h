#pragma once

#include "RenderDataBuffer.h"

namespace LindaEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, int vertexStride, bool isStatic = true);
		OpenGLVertexBuffer(float* vertices, uint32_t size, int vertexStride, bool isStatic = true);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetVertexAttribute(std::vector<VertexAttribute>* attrs) override;
		virtual const std::vector<VertexAttribute>* GetAttributes() const override;
		virtual uint32_t GetCount() const override { return _count; };
		virtual void SetData(void* data, uint32_t size) override;

	private:
		unsigned int _rendererID;
		uint32_t _count;
		std::vector<VertexAttribute>* attributes;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count, bool is32Bit = true, bool isStatic = true);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return _count; }

	private:
		unsigned int _rendererID;
		uint32_t _count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer, int vertexStride) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const { return _vertexBuffer; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }

		virtual void Draw(DrawType drawType) override;

		virtual void SetEmptyState() override { _isEmpty = true; }

	private:
		unsigned int _rendererID;
		Ref<VertexBuffer> _vertexBuffer;
		Ref<IndexBuffer> _indexBuffer;

		bool _isEmpty = false;
	};
}
