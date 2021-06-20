#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "BasicShapes.h"

namespace Saturn
{
	struct BindingInfo
	{
		uint32_t infoCount;
		VkVertexInputBindingDescription* bindingDescription;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		BindingInfo GetBindingInfo();

	private:
		VkVertexInputBindingDescription bindingDescription = {};
	};

}

#endif VERTEXBUFFER_H