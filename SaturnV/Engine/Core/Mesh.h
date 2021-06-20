#ifndef MESH_H
#define MESH_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include "BasicShapes.h"
#include "../Log/Log.h"

namespace Saturn
{

	class Mesh
	{
	public:
		Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, std::vector<Vertex> vertices);
		void FreeResources();

		inline uint32_t GetVertexCount() { return m_VertexCount; }
		inline VkBuffer GetVertexBuffer() { return m_VertexBuffer; }

	private:
		uint32_t m_VertexCount;
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_DeviceMemory;

		std::vector<Vertex> m_Vertices;

		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;

		void CreateVertexBuffer();
		uint32_t FindMemoryType(uint32_t allowedTypes, VkMemoryPropertyFlags memoryPropertyFlags);
	};


}

#endif // !MESH_H