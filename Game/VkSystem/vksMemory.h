#pragma once
#define VMA_IMPLEMENTATION
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace vks{
    class Memory{
    public:
        Memory(VmaAllocator allocator);
        void Dispose();
        VkBuffer buffer = nullptr;
        VkImage image = nullptr;
        VmaAllocationInfo allocationInfo = {};
        void SetAsBuffer(VkBufferCreateInfo& bcInfoRef, VmaAllocationCreateInfo& acInfoRef);
        //void TransferFromRam(void* data, uint64_t offset, uint64_t size, uint64_t ramOffset = 0);
        //void TransferToBuffer(VkCommandBuffer cmd, Memory& dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size);
        //void TransferFromBuffer(VkCommandBuffer cmd, Memory& src, uint64_t srcOffset, uint64_t dstOffset, uint64_t size);
        void SetAsImage(VkImageCreateInfo& icInfoRef, VmaAllocationCreateInfo& acInfoRef);
        ~Memory(){Dispose();}
    private:
        VmaAllocator allocator;
        VkBufferCreateInfo bcInfo = {};
        VkImageCreateInfo icInfo = {};
        VmaAllocationCreateInfo acInfo = {};
        VmaAllocation allocation = nullptr;
    };

}
