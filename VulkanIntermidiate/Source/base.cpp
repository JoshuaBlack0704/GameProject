#define VMA_IMPLEMENTATION
#include "../Include/Storage.hpp"
#include "../Include/API.hpp"
#include <spdlog/spdlog.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void VulkanInitalizer::Init(){
    
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    data = new MainData();
    data->window = glfwCreateWindow(100, 100, "title", nullptr, nullptr);

    auto instret = vkb::InstanceBuilder().enable_validation_layers().require_api_version(VK_API_VERSION_1_3).build();
    if (!instret)
    {
        spdlog::info("Could not create instance");
        abort();
    }
    data->instance = instret.value();
    if (glfwCreateWindowSurface(data->instance, data->window, nullptr, (VkSurfaceKHR*)&data->surface) != VkResult::VK_SUCCESS)
    {
        spdlog::error("\nCould not create glfw surface\nInstance pointer: {}\nWindow pointer: {}\nOutput surface pointer: {}\n", (uint64_t)(VkInstance)data->instance, (void*)data->window, (uint64_t)(VkSurfaceKHR)data->surface);
        abort();
    }

    VkPhysicalDeviceFeatures features{}; features.samplerAnisotropy = VK_TRUE;
    VkPhysicalDeviceVulkan11Features features11{};
    VkPhysicalDeviceVulkan12Features features12{}; features12.timelineSemaphore = VK_TRUE; 
    VkPhysicalDeviceVulkan13Features features13{}; features13.dynamicRendering = VK_TRUE;

    auto pdevret = vkb::PhysicalDeviceSelector(data->instance)
    .set_surface(data->surface)
    .add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
    .set_required_features(features)
    .set_required_features_11(features11)
    .set_required_features_12(features12)
    .set_required_features_13(features13)
    .require_present()
    .select();


    spdlog::info("Selected device named: {} \n", pdevret->properties.deviceName);
    if (!pdevret)
    {
        spdlog::info("ERROR CODE OCCURRED: {} \n", pdevret.vk_result());
        abort();
    }

    data->pDevice = pdevret.value();

    auto ldevret = vkb::DeviceBuilder{data->pDevice}.build();
    if (!ldevret){
        spdlog::info("Failed to create device");
        abort();
    }

    data->device = ldevret.value();

    VmaAllocatorCreateInfo alocCreateInfo;
    alocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    alocCreateInfo.physicalDevice = data->pDevice.physical_device;
    alocCreateInfo.device = data->device.device;
    alocCreateInfo.instance = data->instance;

    vmaCreateAllocator(&alocCreateInfo, &data->allocator);

    spdlog::info("Fin");

    while(!glfwWindowShouldClose(data->window)){
        glfwPollEvents();
    }

}

