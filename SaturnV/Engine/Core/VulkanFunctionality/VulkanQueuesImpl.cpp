#include "VulkanCore.h"

QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef)
{
	QueueFamilyIndices queueFamilyIndices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceRef, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceRef, &queueFamilyCount, queueFamilyList.data());

	for (unsigned int i = 0; i < queueFamilyCount; i++)
	{
		//Check whether the family has at east one queue in the family and also it has to be of type graphics queue
		if (queueFamilyList[i].queueCount > 0 && queueFamilyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamilyIndices.graphicsFamily = i; //graphics queue index is the i value 
		}

		//check if queue family supports presentation
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDeviceRef, i, surfaceRef, &presentationSupport);

		if (queueFamilyList[i].queueCount > 0 && presentationSupport)
		{
			queueFamilyIndices.presentationFamily = i;
		}


		if (queueFamilyIndices.IsValid())
		{
			LOG_PASS("Found suitable queues!");
			return queueFamilyIndices;
		}
	}

	LOG_PASS("Could not find queues!");
	return queueFamilyIndices;
}