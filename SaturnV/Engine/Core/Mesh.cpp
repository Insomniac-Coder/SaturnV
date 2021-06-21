#include "Mesh.h"

Saturn::Mesh::Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, std::vector<Vertex> vertices, std::vector<uint32_t> indices, VkQueue transferQueue, VkCommandPool transferCommandPool)
	: m_Vertices(vertices), m_PhysicalDevice(physicalDevice), m_LogicalDevice(logicalDevice), m_VertexCount(static_cast<uint32_t>(vertices.size())), m_Indices(indices), m_IndexCount(static_cast<uint32_t>(indices.size()))
{
	PopulateVertexBuffer(transferQueue, transferCommandPool);
	PopulateIndexBuffer(transferQueue, transferCommandPool);
}


void Saturn::Mesh::FreeResources()
{
	vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, m_DeviceMemory, nullptr);
	vkDestroyBuffer(m_LogicalDevice, m_IndexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, m_IndexMemory, nullptr);
}

void Saturn::Mesh::PopulateVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;
	
	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_Vertices.size() * sizeof(Vertex), stagingBuffer, stagingMemory, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data; //this will hold the pointer to the memory in VRAM
	vkMapMemory(m_LogicalDevice, stagingMemory, 0, m_Vertices.size() * sizeof(Vertex), 0, &data);
	memcpy(data, m_Vertices.data(), (size_t)(m_Vertices.size() * sizeof(Vertex)));
	vkUnmapMemory(m_LogicalDevice, stagingMemory);

	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_Vertices.size() * sizeof(Vertex), m_VertexBuffer, m_DeviceMemory, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CopyBuffer(m_LogicalDevice, transferQueue, transferCommandPool, stagingBuffer, m_VertexBuffer, m_Vertices.size() * sizeof(Vertex));

	vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, stagingMemory, nullptr);
}

void Saturn::Mesh::PopulateIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;

	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_Indices.size() * sizeof(uint32_t), stagingBuffer, stagingMemory, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data; //this will hold the pointer to the memory in VRAM
	vkMapMemory(m_LogicalDevice, stagingMemory, 0, m_Indices.size() * sizeof(uint32_t), 0, &data);
	memcpy(data, m_Indices.data(), (size_t)(m_Indices.size() * sizeof(uint32_t)));
	vkUnmapMemory(m_LogicalDevice, stagingMemory);

	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, m_Indices.size() * sizeof(uint32_t), m_IndexBuffer, m_IndexMemory, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CopyBuffer(m_LogicalDevice, transferQueue, transferCommandPool, stagingBuffer, m_IndexBuffer, m_Indices.size() * sizeof(uint32_t));

	vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, stagingMemory, nullptr);
}
