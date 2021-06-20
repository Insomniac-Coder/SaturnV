#include "VulkanCore.h"

void CreateBuffer(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkBufferUsageFlags bufferUsageFlag, uint64_t bufferSize, VkBuffer& bufferRef, VkDeviceMemory& memoryRef, VkMemoryPropertyFlags memoryPropertyFlagsRef)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;
	bufferInfo.usage = bufferUsageFlag;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &bufferRef);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Failed to created vertex Buffer");
	}

	//get memory requirements
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, bufferRef, &memoryRequirements);

	//Alocate memory
	VkMemoryAllocateInfo AllocateInfo = {};
	AllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	AllocateInfo.allocationSize = memoryRequirements.size;
	AllocateInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlagsRef);

	//Memory Allocation
	result = vkAllocateMemory(logicalDevice, &AllocateInfo, nullptr, &memoryRef);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Memory Allocation failed!");
	}
}

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t allowedProperties, VkMemoryPropertyFlags requiredPropertyFlags)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((allowedProperties & (1 << i)) && (requiredPropertyFlags & memoryProperties.memoryTypes[i].propertyFlags) == requiredPropertyFlags )
		{
			return i;
		}
	}
	return uint32_t();
}