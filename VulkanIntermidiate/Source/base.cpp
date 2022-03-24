#define VMA_IMPLEMENTATION
#include "../Include/Storage.hpp"
#include "../Include/API.hpp"
#include <spdlog/spdlog.h>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_vulkan.h"

void VulkanInitalizer::Init(bool useImGui)
{

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
    if (glfwCreateWindowSurface(data->instance, data->window, nullptr, (VkSurfaceKHR *) &data->surface) !=
        VkResult::VK_SUCCESS)
    {
        spdlog::error(
                "\nCould not create glfw surface\nInstance pointer: {}\nWindow pointer: {}\nOutput surface pointer: {}\n",
                (uint64_t) (VkInstance) data->instance, (void *) data->window, (uint64_t) (VkSurfaceKHR) data->surface);
        abort();
    }

    VkPhysicalDeviceFeatures features{};
    features.samplerAnisotropy = VK_TRUE;
    VkPhysicalDeviceVulkan11Features features11{};
    VkPhysicalDeviceVulkan12Features features12{};
    features12.timelineSemaphore = VK_TRUE;
    VkPhysicalDeviceVulkan13Features features13{};
    features13.dynamicRendering = VK_TRUE;

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
    if (!ldevret)
    {
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

    if(useImGui)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
        ImGui_ImplGlfw_InitForVulkan(data->window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = data->instance.instance;
        init_info.PhysicalDevice = data->pDevice.physical_device;
        init_info.Device = data->device.device;
        init_info.QueueFamily = data->device.get_queue_index(vkb::QueueType::graphics).value();
        init_info.Queue = data->device.get_queue(vkb::QueueType::graphics).value();
        init_info.PipelineCache = NULL;
        //init_info.DescriptorPool = g_DescriptorPool;
        //init_info.Allocator = g_Allocator;
        //init_info.MinImageCount = g_MinImageCount;
        //init_info.ImageCount = wd->ImageCount;
        //init_info.CheckVkResultFn = check_vk_result;
        //ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
    }


    spdlog::info("Fin");

    while(!glfwWindowShouldClose(data->window)){
        glfwPollEvents();
    }

}

