#define VMA_IMPLEMENTATION
#include "../Include/VkLayer.hpp"
#include <spdlog/spdlog.h>


GLFWwindow* vkl::window = nullptr;
vkb::Instance vkl::instance = vkb::Instance();
vkb::PhysicalDevice vkl::pDevice = vkb::PhysicalDevice();
vkb::Device vkl::lDevice = vkb::Device();
vkb::Swapchain vkl::swapchain = vkb::Swapchain();
VkSurfaceKHR vkl::surface = VkSurfaceKHR();
std::vector<std::function<void(GLFWwindow* window, int width, int height)>> vkl::resizeCallbacks = std::vector<std::function<void(GLFWwindow* window, int width, int height)>>();

//Static settings
int vkl::windowWidth = 100;
int vkl::windowHeight = 100;
bool vkl::useImGui = false;


namespace VK_LAYER_NAMESPACE
{




    void InitVkLayer()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(windowWidth, windowWidth, "Game", nullptr, nullptr);

        auto instanceRet = vkb::InstanceBuilder()
#ifndef NDEBUG
                .enable_validation_layers()
#endif
                .require_api_version(VK_API_VERSION_1_3)
                .build();
        if (!instanceRet)
        {
            spdlog::error("Could not create Instance");
            abort();
        }

        instance = instanceRet.value();

        glfwCreateWindowSurface(instance.instance, window, nullptr,
                                &surface);


        VkPhysicalDeviceFeatures features{};
        features.samplerAnisotropy = VK_TRUE;
        VkPhysicalDeviceVulkan11Features features11{};
        VkPhysicalDeviceVulkan12Features features12{};
        features12.timelineSemaphore = VK_TRUE;
        VkPhysicalDeviceVulkan13Features features13{};
        features13.dynamicRendering = VK_TRUE;

        auto pdevret = vkb::PhysicalDeviceSelector(instance)
                .set_surface(surface)
                .add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                .set_required_features(features)
                .set_required_features_11(features11)
                .set_required_features_12(features12)
                .set_required_features_13(features13)
                .require_present()
                .select();
        if (!pdevret)
        {
            spdlog::error("Could not select a physical device");
            abort();
        }

        pDevice = pdevret.value();

        auto ldevret = vkb::DeviceBuilder(pDevice).build();
        if (!ldevret){
            spdlog::error("Could not build a logical device");
            abort();
        }

        lDevice = ldevret.value();

        auto swapret = vkb::SwapchainBuilder(lDevice).set_old_swapchain(swapchain).build();
        if (!swapret){
            spdlog::error("Could not create swapchain");
            abort();
        }

        for(auto& func : resizeCallbacks){
            func(window, windowWidth, windowHeight);
        }
    }

    void AddResizeCallBack(std::function<void(GLFWwindow *window, int width, int height)> callback)
    {
        resizeCallbacks.emplace_back(callback);
    }

}



