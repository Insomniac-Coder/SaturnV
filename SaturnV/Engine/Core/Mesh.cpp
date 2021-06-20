#include "Mesh.h"

Saturn::Mesh::Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, std::vector<Vertex> vertices)
	: m_Vertices(vertices), m_PhysicalDevice(physicalDevice), m_LogicalDevice(logicalDevice), m_VertexCount(static_cast<uint32_t>(vertices.size()))
{
	CreateVertexBuffer();
}


void Saturn::Mesh::FreeResources()
{
	vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, m_DeviceMemory, nullptr);
}

void Saturn::Mesh::CreateVertexBuffer()
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = m_Vertices.size() * sizeof(Vertex);
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(m_LogicalDevice, &bufferInfo, nullptr, &m_VertexBuffer);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Failed to created vertex Buffer");
	}

	//get memory requirements
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(m_LogicalDevice, m_VertexBuffer, &memoryRequirements);

	//Alocate memory
	VkMemoryAllocateInfo AllocateInfo = {};
	AllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	AllocateInfo.allocationSize = memoryRequirements.size;
	AllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//Memory Allocation
	result = vkAllocateMemory(m_LogicalDevice, &AllocateInfo, nullptr, &m_DeviceMemory);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Memory Allocation failed!");
	}

	vkBindBufferMemory(m_LogicalDevice, m_VertexBuffer, m_DeviceMemory, 0); //binding the buffer to the allocated memory

	void* data; //this will hold the pointer to the memory int VRAM

	vkMapMemory(m_LogicalDevice, m_DeviceMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, m_Vertices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(m_LogicalDevice, m_DeviceMemory);
}

uint32_t Saturn::Mesh::FindMemoryType(uint32_t allowedTypes, VkMemoryPropertyFlags memoryPropertyFlags)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((allowedTypes & (1 << i)) && (memoryPropertyFlags & memoryProperties.memoryTypes[i].propertyFlags) == memoryPropertyFlags)
		{
			return i;
		}
	}
	return uint32_t();
}
