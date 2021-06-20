#ifndef RENDERER_H
#define RENDERER_H
#include "../Log/Log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "FileIO.h"
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "VertexBuffer.h"
#include "VertexAttributes.h"
#include "Mesh.h"

namespace Saturn {

	class Renderer
	{
	public:
		Renderer(unsigned int windowWidth, unsigned int windowHeight, unsigned int virtualWidth, unsigned int virtualHeight, GLFWwindow* windowRef);
		~Renderer();
		void Init();
		void Run();

	private:
		GLFWwindow* r_Window;
		int r_Width;
		int r_Height;
		int r_VirtualWidth;
		int r_VirtualHeight;
		glm::vec4 r_ClearColor = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

		//Vulkan Components
		const std::vector<const char*> r_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation", //contains all useful validations
		};

		const std::vector<const char*> r_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef _DEBUG
		const bool r_EnableValidationLayers = true;
#else
		const bool r_EnableValidationLayers = false;
#endif

		VkInstance r_Instance; // instance of a vulkan program
		struct
		{
			VkPhysicalDevice physicalDevice;
			VkDevice logicalDevice;
		} r_MainDevice;

		struct QueueFamilyIndices
		{
			int graphicsFamily = -1; //location of graphics queue family
			int presentationFamily = -1;

			bool IsValid()
			{
				return graphicsFamily >= 0 && presentationFamily >= 0;
			}
		} r_QueueIndices;

		VkQueue r_GraphicsQueue;
		VkQueue r_PresentationQueue;
		VkSurfaceKHR r_Surface;
		VkSwapchainKHR r_Swapchain;
		VkExtent2D r_Extent2D;
		VkFormat r_ImageFormat;

		struct SurfaceProperties
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			std::vector<VkSurfaceFormatKHR> supportedFormats;
			std::vector<VkPresentModeKHR> presentationModes;
		} r_SurfaceProperties;

		struct SwapchainImage
		{
			VkImage image;
			VkImageView imageView;
		};

		std::vector<SwapchainImage> r_SwapchainImages;
		std::vector<VkFramebuffer> r_SwapChainFrameBuffer;
		std::vector<VkCommandBuffer> r_CommandBuffers;

		VkPipeline r_GraphicsPipeline;
		VkPipelineLayout r_PipelineLayout;
		VkRenderPass r_RenderPass;

		VkCommandPool r_GraphicsCommandPool;

		//Sync
		std::vector<VkSemaphore> r_ImageAvailable;
		std::vector<VkSemaphore> r_RenderFinished;
		std::vector<VkFence> r_DrawFences;

		std::vector<Mesh> r_Meshes;

		//Vulkan specific functions
		bool CheckValidationLayerSupport();

		int CreateInstance(); //creates a vulkan instance
		int CreateSurface();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice* pDevice);
		void GetDetailsforSwapchain(VkPhysicalDevice* pDevice);
		bool CheckInstanceExtensionSupport(std::vector<const char*>* Extensions); // confirms whether the extensions required by GLFW are supported by vulkan
		int GetPhysicalDevice(); //post the creation of Vulkan instance we need to know what physical devices are present on the system
		bool CheckPhysicalDeviceSuitable(VkPhysicalDevice* pDevice); // check whether the selected device is suitable or not
		void GetQueueFamilyIndices(VkPhysicalDevice* pDevice); // Get the indices of queue families present on the GPU
		int CreateLogicalDevice(); //Once we have picked a physical device and confirmed whether it's suitable or not, we will create logical device from it
		VkSurfaceFormatKHR GetBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		VkPresentModeKHR GetBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
		VkExtent2D GetBestSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		int CreateSwapChain();
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		int CreateGraphicsPipeline();
		int CreateRenderPass();
		int CreateFrameBuffers();
		int CreateCommandPool();
		int CreateCommandBuffers();
		int RecordCommands();
		void Draw();
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		int CreateSynchronisation();
		const int MAX_FRAME_DRAWS = 2;
		int currentFrame = 0;
	};

}
#endif RENDERER_H

