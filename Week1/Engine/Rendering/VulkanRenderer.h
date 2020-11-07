#pragma once
#include "Renderer.h"
#include "vulkan/vulkan.h"
#include "SDL_vulkan.h"
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

//Structs
struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();;
	}
};

class VulkanRenderer : public Renderer
{
public:
	SDL_Window* CreateWindow(std::string name_, int width_, int height_) override;

	VulkanRenderer(std::string windowName = "Vulkan", int width = 800, int height = 600);
	~VulkanRenderer();

protected:

	const int MAX_FRAMES_IN_FLIGHT = 2;

	const std::vector<const char*> validationLayers = {
"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


	//Instance
	VkInstance instance;
	VkSurfaceKHR vSurface;

	//Window
	SDL_Window* window;

	//Devices
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures deviceFeatures{};
	std::vector<VkPhysicalDevice> allDevices;

	//Queues
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	//Swap chain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	//Render passes
	VkRenderPass renderPass;
	VkRenderPassCreateInfo renderPassInfo;

	//Pipelines
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	//Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;

	//Command buffers
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	//Semaphores
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;

	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	void OnCreate();

	std::vector<char> readFile(const std::string& filename);

public:
	void CreateInstance();
	void PickPhysicalDevice();
	//CreateLogicalDevice is called inside IsDeviceSutiable
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSyncObjects();
	void FindSuitableDevice();
	bool IsDeviceSutiable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateLogicalDevice(QueueFamilyIndices indices_);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	int RateDeviceSuitability(VkPhysicalDevice device, VkPhysicalDeviceProperties deviceProperties_, VkPhysicalDeviceFeatures deviceFeatures_);
	bool checkValidationLayerSupport();
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void DrawFrame();
	void OnDestroy();
	void Render() override;

};

