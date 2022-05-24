#include <vksVkData.h>
#include <spdlog/spdlog.h>


vks::VkData::VkData(bool useGLFW) {
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

    VkPhysicalDeviceFeatures features = {};
    features.samplerAnisotropy = VK_TRUE;
    VkPhysicalDeviceVulkan11Features features11 = {};
    VkPhysicalDeviceVulkan12Features features12 = {};
    features12.timelineSemaphore = VK_TRUE;
    VkPhysicalDeviceVulkan13Features features13 = {};
    features13.dynamicRendering = VK_TRUE;

    auto pDeviceSel = vkb::PhysicalDeviceSelector(instance);
    pDeviceSel.set_required_features(features);
    pDeviceSel.set_required_features_11(features11);
    pDeviceSel.set_required_features_12(features12);
    pDeviceSel.set_required_features_13(features13);
    pDeviceSel.require_present(false);
    if (useGLFW){
        spdlog::info("Building GLFW");
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(100,100, "Window", nullptr, nullptr);
        glfwCreateWindowSurface(instance.instance, window, nullptr, &surface);

        pDeviceSel.add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        pDeviceSel.set_surface(surface);
        pDeviceSel.require_present();
    }
    auto pDeviceRet = pDeviceSel.select();

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

    if (useGLFW){
        auto swapRet = vkb::SwapchainBuilder(lDevice)
                .set_old_swapchain(swapchain)
                .set_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                .build();
        if (!swapRet){
            spdlog::error("Could not create swapchain");
            abort();
        }
        swapchain = swapRet.value();
    }


    VmaAllocatorCreateInfo alocCreateInfo {};
    alocCreateInfo.instance = instance.instance;
    alocCreateInfo.device = lDevice.device;
    alocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    alocCreateInfo.physicalDevice = pDevice.physical_device;
    vmaCreateAllocator(&alocCreateInfo, &allocator);
}

void vks::VkData::Dispose() {
    vmaDestroyAllocator(allocator);
    if (swapchain.swapchain != VK_NULL_HANDLE){
    vkDestroySwapchainKHR(lDevice.device, swapchain.swapchain, nullptr);
    }
    vkDestroyDevice(lDevice.device, nullptr);
    if (surface != VK_NULL_HANDLE){
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }
    vkDestroyInstance(instance, nullptr);
    spdlog::info("Disposing VkData");
}