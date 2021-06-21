#ifndef MESH_H
#define MESH_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include "BasicShapes.h"
#include "../Log/Log.h"
#include "VulkanFunctionality/VulkanCore.h"

namespace Saturn
{

	class Mesh
	{
	public:
		Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, std::vector<Vertex> vertices, std::vector<uint32_t> indices, VkQueue transferQueue, VkCommandPool transferCommandPool);
		void FreeResources();

		inline uint32_t GetVertexCount() { return m_VertexCount; }
		inline VkBuffer GetVertexBuffer() { return m_VertexBuffer; }
		inline VkBuffer GetIndexBuffer() { return m_IndexBuffer; }
		inline uint32_t GetIndexCount() { return m_IndexCount; }

	private:
		uint32_t m_VertexCount;
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_DeviceMemory;

		uint32_t m_IndexCount;
		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexMemory;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;

		void PopulateVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool);
		void PopulateIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool);
	};


}

#endif // !MESH_H