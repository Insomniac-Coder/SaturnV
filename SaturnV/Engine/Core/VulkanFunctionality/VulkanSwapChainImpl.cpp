#include "VulkanCore.h"

bool CheckDeviceExtensionSupport(VkPhysicalDevice& physicalDeviceRef)
{
	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	uint32_t supportedDeviceExtensionCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDeviceRef, nullptr, &supportedDeviceExtensionCount, nullptr);

	if (supportedDeviceExtensionCount == 0)
	{
		return false;
	}

	std::vector<VkExtensionProperties> supportedDeviceExtensions(supportedDeviceExtensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDeviceRef, nullptr, &supportedDeviceExtensionCount, supportedDeviceExtensions.data());

	for (const char* requiredExtension : deviceExtensions) {
		bool hasSupport = false;
		for (const auto& supportedDeviceExtension : supportedDeviceExtensions)
		{
			if (strcmp(requiredExtension, supportedDeviceExtension.extensionName) == 0)
			{
				hasSupport = true;
				break;
			}
		}

		if (!hasSupport)
		{
			return false;
		}
	}

	return true;
}

SurfaceProperties GetDetailsForSwapChain(VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef)
{
	SurfaceProperties surfaceProperties;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDeviceRef, surfaceRef, &surfaceProperties.surfaceCapabilities);

	uint32_t supportedFormatsCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceRef, surfaceRef, &supportedFormatsCount, nullptr);
	std::vector<VkSurfaceFormatKHR> supportedFormats(supportedFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceRef, surfaceRef, &supportedFormatsCount, supportedFormats.data());

	uint32_t supportedPresentationModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceRef, surfaceRef, &supportedPresentationModeCount, nullptr);
	std::vector<VkPresentModeKHR> supportedPresentationModes(supportedPresentationModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceRef, surfaceRef, &supportedPresentationModeCount, supportedPresentationModes.data());

	surfaceProperties.supportedFormats = supportedFormats;
	surfaceProperties.presentationModes = supportedPresentationModes;

	return surfaceProperties;
}