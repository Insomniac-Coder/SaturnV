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

	vkBindBufferMemory(logicalDevice, bufferRef, memoryRef, 0); //binding the buffer to the allocated memory
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

void CopyBuffer(VkDevice logicalDevice, VkQueue queue, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
{
	VkCommandBuffer transferCommandBuffer;

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandPool = commandPool;
	allocateInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &transferCommandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(transferCommandBuffer, &beginInfo);

	//region data to copy from and to
	VkBufferCopy bufferCopyRegion = {};
	bufferCopyRegion.srcOffset = 0;
	bufferCopyRegion.dstOffset = 0;
	bufferCopyRegion.size = bufferSize;

	vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopyRegion);

	vkEndCommandBuffer(transferCommandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transferCommandBuffer;

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &transferCommandBuffer);
}