#include "VertexBuffer.h"

Saturn::VertexBuffer::VertexBuffer()
{
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

Saturn::BindingInfo Saturn::VertexBuffer::GetBindingInfo()
{
	return { 1, &bindingDescription };
}
