#include "UniformDataGlobal.h"
#include "UniformBuffer.h"

using namespace LindaEngine;

UniformDataGlobal::UniformDataGlobal()
{
	size = sizeof(Data);
	buffer = UniformBuffer::Create(size, 0);
}

void UniformDataGlobal::SetUniformBufferData()
{
	if (buffer == nullptr)
		return;
	buffer->SetData(&data, size);
}
