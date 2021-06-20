#include "VertexAttributes.h"

Saturn::VertexAttributes::VertexAttributes()
{

	attributeDescriptions.resize(2);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = ((size_t) & (((Vertex*)0)->Position));

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = ((size_t) & (((Vertex*)0)->Color));
}

Saturn::AttributesInfo Saturn::VertexAttributes::GetAttributesInfo()
{
	return { static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data() };
}
