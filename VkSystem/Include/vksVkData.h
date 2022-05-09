#pragma once
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
#include <functional>

namespace vks
{
    struct vksVkData
    {
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

