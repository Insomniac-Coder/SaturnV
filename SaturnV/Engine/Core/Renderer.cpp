#include "Renderer.h"
#include <set>

Saturn::Renderer::Renderer(unsigned int windowWidth, unsigned int windowHeight, unsigned int virtualWidth, unsigned int virtualHeight, GLFWwindow* windowRef) : r_Width(windowWidth), r_Height(windowHeight), r_VirtualWidth(virtualWidth), r_VirtualHeight(virtualHeight), r_Window(windowRef)
{
	glfwMakeContextCurrent(r_Window);
	//glfwSetWindowSizeCallback(r_Window, WindowResize);
	uint32_t extensionCount = 0;

	//To find how many extensions an "instance" of vulkan will support on current system
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	LOG_INFO("This device supports " + std::to_string(extensionCount) + " Vulkan extensions.");

	Init();
}

Saturn::Renderer::~Renderer()
{
	for (auto image : r_SwapchainImages)
	{
		vkDestroyImageView(r_MainDevice.logicalDevice, image.imageView, nullptr);
	}

	vkDestroySwapchainKHR(r_MainDevice.logicalDevice, r_Swapchain, nullptr);
	vkDestroySurfaceKHR(r_Instance, r_Surface, nullptr);
	vkDestroyDevice(r_MainDevice.logicalDevice, nullptr);
	vkDestroyInstance(r_Instance, nullptr);
}

void Saturn::Renderer::Init()
{
	if(CreateInstance() == EXIT_SUCCESS)
	{
		if (CreateSurface() == EXIT_SUCCESS) {
			LOG_PASS("Surface created!");
			LOG_INFO("Fetching list of physical devices on system(GPU)");
			if (GetPhysicalDevice() == EXIT_SUCCESS)
			{
				if (CreateLogicalDevice() == EXIT_SUCCESS)
				{
					LOG_PASS("Logical device created!");
					if (CreateSwapChain() == EXIT_SUCCESS)
					{
						LOG_PASS("Swapchain created successfully!");
						if (CreateGraphicsPipeline() == EXIT_SUCCESS)
						{
							LOG_PASS("Graphics pipeline created successfully!");
						}
						else
						{
							LOG_FAIL("Failed to create graphics pipeline!");
						}
					}
					else
					{
						LOG_FAIL("Swapchain creation failed!");
					}
				}
				else
				{
					LOG_FAIL("Logical device creation failed");
				}
			}
			else
			{
				LOG_FAIL("[ERROR] Failed to find any physical devices on your system!");
			}
		}
		else
		{
			LOG_FAIL("Failed to create surface");
		}
	}
	else
	{
		LOG_FAIL("[ERROR] Vulkan instance creation failed!");
	}
}

void Saturn::Renderer::Run()
{

	while (!glfwWindowShouldClose(r_Window)) {
		glfwPollEvents();
	}

	std::string fileContent = "WindowWidth : " + std::to_string(r_Width) + "\n" + "WindowHeight : " + std::to_string(r_Height) + "\n" + "VirtualRenderWidth : " + std::to_string(r_VirtualWidth) + "\n" + "VirtualRenderHeight : " + std::to_string(r_VirtualHeight) + "\n";
	WriteFile("Config.Saturn", fileContent);
	LOG_PASS("Latest file config saved!");
	LOG_PASS("DEBUG Content:\n" + fileContent);
}

bool Saturn::Renderer::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> supportedLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data());

	for (int i = 0; i < r_ValidationLayers.size(); i++)
	{
		bool hasSupport = false;
		for (int x = 0; x < layerCount; x++)
		{
			if (strcmp(r_ValidationLayers[i], supportedLayers[x].layerName) == 0)
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

int Saturn::Renderer::CreateInstance()
{
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
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	if (r_EnableValidationLayers) {
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	if (!CheckInstanceExtensionSupport(&instanceExtensions))
	{
		return EXIT_FAILURE;
	}
	else
	{
		LOG_PASS("Vulkan supports all extensions required by GLFW!");
	}

	if (!CheckValidationLayerSupport())
	{
		LOG_FAIL("Not all Validation Layers are supported");
		return EXIT_FAILURE;
	}
	else
	{
		LOG_PASS("The requested Validation layer is supported by Vulkan!");
	}

	//instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceCreateInfo.enabledExtensionCount = glfwExtensionCount; // more convenient way of passing count
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	if (r_EnableValidationLayers) {
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(r_ValidationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = r_ValidationLayers.data();
	}
	else
	{
		instanceCreateInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &r_Instance);

	if (result != VK_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}

//Compare the list of required extensions required by GLFW with the list of extensions the Vulkan supports
bool Saturn::Renderer::CheckInstanceExtensionSupport(std::vector<const char*>* Extensions)
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

int Saturn::Renderer::GetPhysicalDevice()
{
	//Enumeration of physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(r_Instance, &deviceCount, nullptr);
	
	//If no devices are available then return failure
	if (deviceCount == 0)
	{
		return EXIT_FAILURE;
	}

	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(r_Instance, &deviceCount, deviceList.data()); //once the size is found populate it with device details
	LOG_INFO("Found " + std::to_string(deviceCount) + " device(s), finding the suitable one");

	for (int i = 0; i < deviceCount; i++)
	{
		if (CheckPhysicalDeviceSuitable(&deviceList[i]))
		{
			LOG_PASS("Found the suitable device!");
			r_MainDevice.physicalDevice = deviceList[i];
			break;
		}
	}

	return EXIT_SUCCESS;
}

bool Saturn::Renderer::CheckPhysicalDeviceSuitable(VkPhysicalDevice* pDevice)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(*pDevice, &deviceProperties);

	LOG_INFO("Device Name: " + (std::string)deviceProperties.deviceName);
	LOG_INFO("Device driver version: " + std::to_string(deviceProperties.driverVersion));
	LOG_INFO("Device API version: " + std::to_string(deviceProperties.apiVersion));

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(*pDevice, &deviceFeatures);

	//Fetch graphics queue family index
	GetQueueFamilyIndices(pDevice);

	bool extensioSupportStatus = CheckDeviceExtensionSupport(pDevice);

	GetDetailsforSwapchain(pDevice);

	return r_QueueIndices.IsValid() && extensioSupportStatus && !r_SurfaceProperties.presentationModes.empty() && !r_SurfaceProperties.supportedFormats.empty();
}

void Saturn::Renderer::GetQueueFamilyIndices(VkPhysicalDevice* pDevice)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*pDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(*pDevice, &queueFamilyCount, queueFamilyList.data());

	for (int i = 0; i < queueFamilyCount; i++)
	{
		//Check whether the family has at east one queue in the family and also it has to be of type graphics queue
		if (queueFamilyList[i].queueCount > 0 && queueFamilyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			r_QueueIndices.graphicsFamily = i; //graphics queue index is the i value 
		}

		//check if queue family supports presentation
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(*pDevice, i, r_Surface, &presentationSupport);

		if (queueFamilyList[i].queueCount > 0 && presentationSupport)
		{
			r_QueueIndices.presentationFamily = i;
		}


		if (r_QueueIndices.IsValid())
		{
			break;
		}
	}
}

int Saturn::Renderer::CreateLogicalDevice()
{
	std::set<int> queueFamilyIndices = { r_QueueIndices.graphicsFamily, r_QueueIndices.presentationFamily };
	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

	//Queues for the logical device
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		deviceQueueCreateInfo.queueCount = 1;
		float priority = 1.0f;
		deviceQueueCreateInfo.pQueuePriorities = &priority;

		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
	}

	//Logical device
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(r_DeviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = r_DeviceExtensions.data();
	
	VkPhysicalDeviceFeatures deviceFeatures = {}; //physical device pfeatures that logical device will use
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	VkResult result = vkCreateDevice(r_MainDevice.physicalDevice, &deviceCreateInfo, nullptr, &r_MainDevice.logicalDevice);

	if (result != VK_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	vkGetDeviceQueue(r_MainDevice.logicalDevice, r_QueueIndices.graphicsFamily, 0, &r_GraphicsQueue);
	vkGetDeviceQueue(r_MainDevice.logicalDevice, r_QueueIndices.presentationFamily, 0, &r_PresentationQueue);
	return EXIT_SUCCESS;
}

int Saturn::Renderer::CreateSwapChain()
{
	VkSurfaceFormatKHR surfaceFormat =  GetBestSurfaceFormat(r_SurfaceProperties.supportedFormats);
	VkPresentModeKHR presentationMode = GetBestPresentationMode(r_SurfaceProperties.presentationModes);
	VkExtent2D extent = r_Extent2D = GetBestSwapExtent(r_SurfaceProperties.surfaceCapabilities);

	r_ImageFormat = surfaceFormat.format;

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.presentMode = presentationMode;
	uint32_t minImage = std::min(r_SurfaceProperties.surfaceCapabilities.minImageCount + 1, r_SurfaceProperties.surfaceCapabilities.maxImageCount);
	swapChainCreateInfo.minImageCount = r_SurfaceProperties.surfaceCapabilities.maxImageCount > 0 ? minImage : r_SurfaceProperties.surfaceCapabilities.minImageCount + 1;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.preTransform = r_SurfaceProperties.surfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.surface = r_Surface;

	if (r_QueueIndices.graphicsFamily != r_QueueIndices.presentationFamily)
	{
		uint32_t indices[] = {
			(uint32_t)r_QueueIndices.graphicsFamily,
			(uint32_t)r_QueueIndices.presentationFamily
		};
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = indices;
	}
	else
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(r_MainDevice.logicalDevice, &swapChainCreateInfo, nullptr, &r_Swapchain);

	uint32_t swapChainImageCount = 0;
	vkGetSwapchainImagesKHR(r_MainDevice.logicalDevice, r_Swapchain, &swapChainImageCount, nullptr);
	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(r_MainDevice.logicalDevice, r_Swapchain, &swapChainImageCount, images.data());

	for (VkImage image : images)
	{
		SwapchainImage swapChainImage;
		swapChainImage.image = image;
		swapChainImage.imageView = CreateImageView(image, r_ImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);

		r_SwapchainImages.push_back(swapChainImage);

	}

	if (result != VK_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}

VkSurfaceFormatKHR Saturn::Renderer::GetBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats)
{
	if (surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& surfaceFormat : surfaceFormats)
	{
		if (((surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM) || (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)) && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return surfaceFormat;
		}
	}

	return surfaceFormats[0];
}

VkPresentModeKHR Saturn::Renderer::GetBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes)
{
	for (const auto& presentationMode : presentationModes)
	{
		if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentationMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Saturn::Renderer::GetBestSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return surfaceCapabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(r_Window, &width, &height);

		VkExtent2D newExtent = {};
		newExtent.width = static_cast<uint32_t>(width);
		newExtent.height = static_cast<uint32_t>(height);

		newExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, newExtent.width));
		newExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, newExtent.height));
	}
}

VkImageView Saturn::Renderer::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.image = image;
	viewCreateInfo.format = format;
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	viewCreateInfo.subresourceRange.aspectMask = aspectFlags;
	viewCreateInfo.subresourceRange.baseMipLevel = 0;
	viewCreateInfo.subresourceRange.levelCount = 1;
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
	viewCreateInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;

	VkResult result = vkCreateImageView(r_MainDevice.logicalDevice, &viewCreateInfo, nullptr, &imageView);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Failed to create image view");
	}

	return imageView;
}

int Saturn::Renderer::CreateGraphicsPipeline()
{
	auto vertexShaderCode = ReadFile_s("Shaders/vert.spv");
	auto fragmentShaderCode = ReadFile_s("Shaders/frag.spv");

	VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
	VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderCreateInfo.module = vertexShaderModule;
	vertexShaderCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderCreateInfo.module = fragmentShaderModule;
	fragmentShaderCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo }; 

	//TODO : Put in vertex descriptions
	//Vertex Input
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;

	//Input Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;
	
	//View port and scissor
	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = (float)r_Extent2D.width;
	viewport.height = (float)r_Extent2D.height;
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

	//Scissor info struct
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = r_Extent2D;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	vkDestroyShaderModule(r_MainDevice.logicalDevice, vertexShaderModule, nullptr);
	vkDestroyShaderModule(r_MainDevice.logicalDevice, fragmentShaderModule, nullptr);

	return 0;
}

VkShaderModule Saturn::Renderer::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};

	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(r_MainDevice.logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule);

	if (result != VK_SUCCESS)
	{
		LOG_FAIL("Failed to create shader module!");
	}

	return shaderModule;
}

int Saturn::Renderer::CreateSurface()
{
	VkResult result = glfwCreateWindowSurface(r_Instance, r_Window, nullptr, &r_Surface);

	if (result != VK_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

bool Saturn::Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice* pDevice)
{
	uint32_t supportedDeviceExtensionCount = 0;
	vkEnumerateDeviceExtensionProperties(*pDevice, nullptr, &supportedDeviceExtensionCount, nullptr);

	if (supportedDeviceExtensionCount == 0)
	{
		return false;
	}

	std::vector<VkExtensionProperties> supportedDeviceExtensions(supportedDeviceExtensionCount);
	vkEnumerateDeviceExtensionProperties(*pDevice, nullptr, &supportedDeviceExtensionCount, supportedDeviceExtensions.data());

	for (const char* requiredExtension : r_DeviceExtensions) {
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

void Saturn::Renderer::GetDetailsforSwapchain(VkPhysicalDevice* pDevice)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*pDevice, r_Surface, &r_SurfaceProperties.surfaceCapabilities);

	uint32_t supportedFormatsCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*pDevice, r_Surface, &supportedFormatsCount, nullptr);
	std::vector<VkSurfaceFormatKHR> supportedFormats(supportedFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(*pDevice, r_Surface, &supportedFormatsCount, supportedFormats.data());

	uint32_t supportedPresentationModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*pDevice, r_Surface, &supportedPresentationModeCount, nullptr);
	std::vector<VkPresentModeKHR> supportedPresentationModes(supportedPresentationModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(*pDevice, r_Surface, &supportedPresentationModeCount, supportedPresentationModes.data());

	r_SurfaceProperties.supportedFormats = supportedFormats;
	r_SurfaceProperties.presentationModes = supportedPresentationModes;

}
