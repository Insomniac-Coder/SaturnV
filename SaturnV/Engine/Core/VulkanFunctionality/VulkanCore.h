#ifndef VULKANCORE_H
#define VULKANCORE_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "../../Log/Log.h"
#include <vector>

//TODO: Move all the complex vulkan implementation to VulkanCore
// Step 1
//Create Vulkan Instance
void CreateInstance(VkInstance& instanceRef); //Creates an instance of Vulkan API
bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers); //Checking whether validation is supported by vulkan, only useful for debugging purposes
bool CheckInstanceExtensionSupport(std::vector<const char*>* Extensions); //Check whether the instance supports the extensions required by GLFW

//Step 2
//Create surface
void CreateSurface(VkInstance instanceRef, GLFWwindow* windowPtr, VkSurfaceKHR& surfaceRef);

//Step 3
//STRUCT that will hold the indices of graphics and presentation queue
struct QueueFamilyIndices
{
	int graphicsFamily = -1; //location of graphics queue family
	int presentationFamily = -1;
	int transferFamily = -1;

	bool IsValid()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0 && transferFamily >=0;
	}
};

struct SurfaceProperties
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> supportedFormats;
	std::vector<VkPresentModeKHR> presentationModes;
};

struct DeviceProperties
{
	QueueFamilyIndices queueFamilyIndices;
	SurfaceProperties surfaceProperties;
};

//Get list of physical devices present in the system and find out which one is suitable
DeviceProperties GetPhysicalDevice(VkInstance& instanceRef, VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef);
bool CheckPhysicalDeviceSuitable(VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef, DeviceProperties& devicePropertiesRef);

//Step 3.5
//Get the required queue family details, swapchain details and also confirm whether the device supports extensions that are required for presentation
//This is required for confirming whether device is suitable for use or not
QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef); // Get the indices of queue families present on the GPU
bool CheckDeviceExtensionSupport(VkPhysicalDevice& physicalDeviceRef);
SurfaceProperties GetDetailsForSwapChain(VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef);

//Buffer Creation and memory allocation
void CreateBuffer(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkBufferUsageFlags bufferUsageFlag, uint64_t bufferSize, VkBuffer& bufferRef, VkDeviceMemory& memoryRef, VkMemoryPropertyFlags memoryPropertyFlagsRef);
uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t allowedProperties, VkMemoryPropertyFlags requiredPropertyFlags);

//Buffer Copying function
void CopyBuffer(VkDevice logicalDevice, VkQueue queue, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize);

#endif VULKANCORE_H