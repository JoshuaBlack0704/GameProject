#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace vks
{
    class Image {
    public:
        Image(VkDevice device, VmaAllocator allocator, VkImageCreateInfo& cInfo);
        void Dispose();
        ~Image(){Dispose();}
    private:
        VkImageCreateInfo cInfo;
        VmaAllocator allocator;
        VkImage image;
        VmaAllocationInfo aInfo;
        VmaAllocation allocation;

    };
}
