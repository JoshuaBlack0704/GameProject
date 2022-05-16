#pragma once
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace vks
{
    struct VkData
    {
        GLFWwindow* window = nullptr;
        vkb::Instance instance;
        VkSurfaceKHR surface = nullptr;
        vkb::PhysicalDevice pDevice;
        vkb::Device lDevice;
        vkb::Swapchain swapchain;
        VmaAllocator allocator = nullptr;
        VkData(bool useGLFW);
        void Dispose();

        ~VkData(){Dispose();}
    };
}

