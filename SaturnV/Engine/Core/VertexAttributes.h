#ifndef VERTEXATTRIBUTE_H
#define VERTEXATTRIBUTE_H

#include <vector>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "BasicShapes.h"

namespace Saturn
{
	struct  AttributesInfo
	{
		uint32_t attributeCount;
		VkVertexInputAttributeDescription* pAttributeDescriptions;
	};

	class VertexAttributes
	{
	public:
		VertexAttributes();
		AttributesInfo GetAttributesInfo();

	private:
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	};

}

#endif VERTEXATTRIBUTE_H