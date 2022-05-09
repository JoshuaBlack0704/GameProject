#define VMA_IMPLEMENTATION
#include <vksVkData.h>
#include <spdlog/spdlog.h>


vks::VkData::VkData(VkSurfaceKHR surface) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(100, 100, "Game", nullptr, nullptr);
    auto instRet = vkb::InstanceBuilder()
            .require_api_version(VK_API_VERSION_1_3)
            .set_app_name("GameProject")
            .set_app_version(0,0,1)
            .set_engine_name("VkData")
            .set_engine_version(0,0,1)
#ifndef NDEBUG
            .enable_validation_layers()
            .build();
    spdlog::info("Enabling validation layers");
#else
    .build();
#endif
    if (!instRet){
        spdlog::error("Could not make a Vulkan Instance");
        abort();
    }

    instance = instRet.value();

    glfwCreateWindowSurface(instance.instance, window, nullptr, &surface);

    VkPhysicalDeviceFeatures features{};
    features.samplerAnisotropy = VK_TRUE;
    VkPhysicalDeviceVulkan11Features features11{};
    VkPhysicalDeviceVulkan12Features features12{};
    features12.timelineSemaphore = VK_TRUE;
    VkPhysicalDeviceVulkan13Features features13{};
    features13.dynamicRendering = VK_TRUE;


    auto pDeviceRet = vkb::PhysicalDeviceSelector(instance)
            .set_surface(surface)
            .add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
            .set_required_features(features)
            .set_required_features_11(features11)
            .set_required_features_12(features12)
            .set_required_features_13(features13)
            .require_present()
            .select();
    if (!pDeviceRet){
        spdlog::error("Could not select physical device");
        abort();
    }

    pDevice = pDeviceRet.value();

    auto lDevRet = vkb::DeviceBuilder(pDevice).build();
    if (!lDevRet){
        spdlog::error("Could not build logical device");
        abort();
    }
    lDevice = lDevRet.value();

    auto swapRet = vkb::SwapchainBuilder(lDevice)
            .set_old_swapchain(swapchain)
            .build();
    if (!swapRet){
        spdlog::error("Could not create swapchain");
        abort();
    }

    swapchain = swapRet.value();

    resizeCallback = [&](){
        auto swapRet = vkb::SwapchainBuilder(lDevice)
                .set_old_swapchain(swapchain)
                .build();
        if (!swapRet){
            spdlog::error("Could not create swapchain");
            abort();
        }
        swapchain = swapRet.value();
    };


    VmaAllocatorCreateInfo alocCreateInfo {};
    alocCreateInfo.instance = instance.instance;
    alocCreateInfo.device = lDevice.device;
    alocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    alocCreateInfo.physicalDevice = pDevice.physical_device;
    vmaCreateAllocator(&alocCreateInfo, &allocator);
}

void vks::VkData::Dispose() {
    vmaDestroyAllocator(allocator);
    vkDestroySwapchainKHR(lDevice.device, swapchain.swapchain, nullptr);
    vkDestroyDevice(lDevice.device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    spdlog::info("Disposing VkData");
}