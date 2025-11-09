#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <vector>

namespace LindaEngine
{
	struct VertexAttribute;

	class VertexBuffer
	{
	public:
		~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetVertexAttribute(std::vector<VertexAttribute>* attrs) = 0;
		virtual const std::vector<VertexAttribute>* GetAttributes() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size, int vertexStride);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size, int vertexStride);
	};

	class IndexBuffer
	{
	public:
		~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

	class VertexArray
	{
	public:
		~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer, int vertexStride) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual void Draw(DrawType drawType) = 0;

		virtual void SetEmptyState() = 0;

		static Ref<VertexArray> Create();
	};
}
