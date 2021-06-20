#include "VulkanCore.h"

void CreateInstance(VkInstance& instanceRef)
{
#ifdef  _DEBUG
	const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation", //contains all useful validations
	};
#endif 

	//Information about the app
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VULKAN APP";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Saturn Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	//Instance of Vulkan program
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;

	//list to hold all extension values
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	uint32_t glfwExtensionCount = 0; //GLFW may require multiple extensions
	const char** glfwExtensions; //list of extensions required by GLFW

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

#ifdef _DEBUG
	instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	if (!CheckInstanceExtensionSupport(&instanceExtensions))
	{
		LOG_FAIL("Vulkan doesn not supports all extensions required by GLFW!");
	}
	else
	{
		LOG_PASS("Vulkan supports all extensions required by GLFW!");
	}

	//instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceCreateInfo.enabledExtensionCount = glfwExtensionCount; // more convenient way of passing count
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

#ifdef _DEBUG
	if (!CheckValidationLayerSupport(validationLayers))
	{
		LOG_FAIL("Not all Validation Layers are supported!");
	}
	else
	{
		LOG_PASS("The requested Validation layer is supported by Vulkan!");
	}

	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
	instanceCreateInfo.enabledLayerCount = 0;
#endif 

	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instanceRef);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Failed to created Vulkan instance!");
	}
	else
	{
		LOG_PASS("Created Vulkan instance!");
	}
}

bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> supportedLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data()); //Find out which Validation layers are supported by device and then find out whether it supports the one that we require

	for (int i = 0; i < validationLayers.size(); i++)
	{
		bool hasSupport = false;
		for (int x = 0; x < layerCount; x++)
		{
			if (strcmp(validationLayers[i], supportedLayers[x].layerName) == 0)
			{
				hasSupport = true;
				break;
			}
		}
		if (!hasSupport)
		{
			LOG_PASS("Cool!");
			return false;
		}
	}

	return true;
}

bool CheckInstanceExtensionSupport(std::vector<const char*>* Extensions) 
{
	//first get the number of supported extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	//Create a list of vkExtensionProperties using the count
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	for (int i = 0; i < Extensions->size(); i++)
	{
		bool hasExtension = false;
		for (int y = 0; y < extensions.size(); y++)
		{
			if (strcmp((*Extensions)[i], extensions[y].extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		if (!hasExtension)
		{
			return false;
		}
	}

	return true;
}