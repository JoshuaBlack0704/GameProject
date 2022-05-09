#pragma once
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>

namespace vks
{
    struct VkData
    {
        VkSurfaceKHR surface;
        vkb::Instance instance;
        vkb::PhysicalDevice pDevice;
        vkb::Device lDevice;
        vkb::Swapchain swapchain;
        VmaAllocator allocator;
        VkData(VkSurfaceKHR surface = nullptr);
        void Dispose();

        ~VkData(){Dispose();}
    };
}

