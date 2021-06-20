#include "Mesh.h"

Saturn::Mesh::Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, std::vector<Vertex> vertices)
	: m_Vertices(vertices), m_PhysicalDevice(physicalDevice), m_LogicalDevice(logicalDevice), m_VertexCount(static_cast<uint32_t>(vertices.size()))
{
	PopulateVertexBuffer();
}


void Saturn::Mesh::FreeResources()
{
	vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, m_DeviceMemory, nullptr);
}

void Saturn::Mesh::PopulateVertexBuffer()
{
	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_Vertices.size() * sizeof(Vertex), m_VertexBuffer, m_DeviceMemory, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	vkBindBufferMemory(m_LogicalDevice, m_VertexBuffer, m_DeviceMemory, 0); //binding the buffer to the allocated memory

	void* data; //this will hold the pointer to the memory int VRAM

	vkMapMemory(m_LogicalDevice, m_DeviceMemory, 0, m_Vertices.size() * sizeof(Vertex), 0, &data);
	memcpy(data, m_Vertices.data(), (size_t)(m_Vertices.size() * sizeof(Vertex)));
	vkUnmapMemory(m_LogicalDevice, m_DeviceMemory);
}