#include "VulkanCore.h"

void CreateSurface(VkInstance instanceRef, GLFWwindow* windowPtr, VkSurfaceKHR& surfaceRef)
{
	VkResult result = glfwCreateWindowSurface(instanceRef, windowPtr, nullptr, &surfaceRef);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Surface creation failed!");
	}

	LOG_PASS("Surface created successfully!");
}