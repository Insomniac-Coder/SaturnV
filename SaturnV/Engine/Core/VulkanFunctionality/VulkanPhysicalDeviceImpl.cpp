#include "VulkanCore.h"

DeviceProperties GetPhysicalDevice(VkInstance& instanceRef, VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef)
{
	LOG_INFO("Fetching list of physical devices on system(GPU)");
	//Enumeration of physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instanceRef, &deviceCount, nullptr);

	//If no devices are available then return failure
	if (deviceCount == 0)
	{
		LOG_FAIL("No physical devices found");
	}

	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instanceRef, &deviceCount, deviceList.data()); //once the size is found populate it with device details
	LOG_INFO("Found " + std::to_string(deviceCount) + " device(s), finding the suitable one");

	DeviceProperties deviceProperties;

	for (unsigned int i = 0; i < deviceCount; i++)
	{
		if (CheckPhysicalDeviceSuitable(deviceList[i], surfaceRef, deviceProperties))
		{
			LOG_PASS("Found the suitable device!");
			physicalDeviceRef = deviceList[i];
			break;
		}
	}

	LOG_PASS("Suitable physical device found");

	return deviceProperties;
}

bool CheckPhysicalDeviceSuitable(VkPhysicalDevice& physicalDeviceRef, VkSurfaceKHR& surfaceRef, DeviceProperties& devicePropertiesRef)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDeviceRef, &deviceProperties);

	LOG_INFO("Device Name: " + (std::string)deviceProperties.deviceName);
	LOG_INFO("Device driver version: " + std::to_string(deviceProperties.driverVersion));
	LOG_INFO("Device API version: " + std::to_string(deviceProperties.apiVersion));

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDeviceRef, &deviceFeatures);

	//Fetch graphics queue family index
	QueueFamilyIndices queueFamilyIndices = GetQueueFamilies(physicalDeviceRef, surfaceRef);

	bool extensionSupportStatus = CheckDeviceExtensionSupport(physicalDeviceRef);

	SurfaceProperties surfaceProperties = GetDetailsForSwapChain(physicalDeviceRef, surfaceRef);

	devicePropertiesRef = { queueFamilyIndices, surfaceProperties };

	return queueFamilyIndices.IsValid() && extensionSupportStatus && !surfaceProperties.presentationModes.empty() && !surfaceProperties.supportedFormats.empty();
}