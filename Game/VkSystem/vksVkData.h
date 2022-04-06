#pragma once
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
#include <functional>

namespace vks
{
    struct vksVkData
    {
        GLFWwindow* window = nullptr;
        VkSurfaceKHR surface;
        vkb::Instance instance;
        vkb::PhysicalDevice pDevice;
        vkb::Device lDevice;
        vkb::Swapchain swapchain;
        VmaAllocator allocator;
        std::function<void()> resizeCallback;
        vksVkData();
        void Dispose();

        ~vksVkData(){Dispose();}
    };
}

