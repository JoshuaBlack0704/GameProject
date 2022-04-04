#pragma once
#include <functional>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <flecs.h>
#include <vk_mem_alloc.h>

namespace vks
{
    ///This struct should hold parent data for all other children vks structs created
    ///So try to keep clear of transient often changing data
    struct VkSystemData
    {
        GLFWwindow* window = nullptr;
        VkSurfaceKHR surface;
        vkb::Instance instance;
        vkb::PhysicalDevice pDevice;
        vkb::Device lDevice;
        vkb::Swapchain swapchain;
        flecs::world* world;
        VmaAllocator allocator;
    };

    struct NeedsCreationTag{};

    //Since we are using dynamic rendering for everything else,
    //the imgui system will need its own renderpass and graphics pipeline
    struct ImGuiData
    {
        VkRenderPass renderPass;
        VkPipeline gPipeline;
        VkDescriptorPool pool;
        VkFramebuffer frame;
    };

    struct ResizeCallback
    {
        std::function<void(GLFWwindow* window, int width, int height, flecs::entity e)> callback;
    };

    ///The flecs module system
    struct VkModule
    {
        VkModule(flecs::world& world);
    };

}

