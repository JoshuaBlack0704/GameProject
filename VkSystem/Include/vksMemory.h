#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <string>
#include <vksTimelineSemaphore.h>


namespace vks{
    class Memory{
    public:
        explicit Memory(VmaAllocator allocator, std::string name);
        void Dispose();
        VkBuffer buffer = nullptr;
        VkImage image = nullptr;
        VmaAllocationInfo allocationInfo = {};
        void* mappedData = nullptr;
        void SetAsBuffer(VkBufferCreateInfo& bcInfoRef, VmaAllocationCreateInfo& acInfoRef);
        void TransferFromRam(const void *data, uint64_t srcOffset, uint64_t size, uint64_t dstOffset);
        void TransferToBuffer(VkCommandBuffer cmd, Memory &dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size, bool barrier);
        void TransferFromBuffer(VkCommandBuffer cmd, Memory &src, uint64_t srcOffset, uint64_t dstOffset, uint64_t size, bool barrier);
        void SetAsImage(VkImageCreateInfo& icInfoRef, VmaAllocationCreateInfo& acInfoRef);
        void FlushAndInvalidate();
        void Map();
        void UnMap();
        void EnsureCapacity(VkCommandBuffer cmd, TimelineSemaphore &signal, uint64_t size);
        void Resize(VkCommandBuffer cmd, TimelineSemaphore &signal, uint64_t size);
        ~Memory(){Dispose();}
    private:
        VmaAllocator allocator = nullptr;
        VkBufferCreateInfo bcInfo = {};
        VkImageCreateInfo icInfo = {};
        VmaAllocationCreateInfo acInfo = {};
        VmaAllocation allocation = nullptr;
        std::string name = {};
    };

}
