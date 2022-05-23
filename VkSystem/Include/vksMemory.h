#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <string>
#include <vksTimelineSemaphore.h>
#include <vksDescriptors.h>

namespace vks{
    class Memory{
    public:
        explicit Memory(VkDevice device, VmaAllocator allocator, std::string name);
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
        void AttachBufferBinding(DescriptorSet& targetSet, uint64_t offset = 0, uint64_t range = 0);
        void AttachImageBinding(DescriptorSet& targetSet, VkImageView view, VkImageLayout layout, VkSampler sampler);
        uint64_t SizeInUse();
        ~Memory(){Dispose();}
    private:
        VkDevice device;
        VmaAllocator allocator = nullptr;
        VkBufferCreateInfo bcInfo = {};
        VkImageCreateInfo icInfo = {};
        VmaAllocationCreateInfo acInfo = {};
        VmaAllocation allocation = nullptr;
        std::string name = {};
    };

}
