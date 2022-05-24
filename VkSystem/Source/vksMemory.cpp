#define VMA_IMPLEMENTATION
#include <vksMemory.h>
#include <vksTypes.h>
#include <spdlog/spdlog.h>
#include <assert.h>
#include <thread>

namespace vks
{
    Memory::Memory(VkDevice device, VmaAllocator allocator, std::string name) : device(device), allocator(allocator), name(name) {}

    void Memory::Dispose() {
        if (buffer != nullptr){
            vmaDestroyBuffer(allocator, buffer, allocation);
            allocation = nullptr;
            buffer = nullptr;
            spdlog::info("Destroying a buffer in memory class \"{}\"", name);
        }
        if (image != nullptr){
            vmaDestroyImage(allocator, image, allocation);
            allocation = nullptr;
            image = nullptr;
            spdlog::info("Destroying an image in memory class \"{}\"", name);
        }
    }

    void Memory::SetAsBuffer(VkBufferCreateInfo &bcInfoRef, VmaAllocationCreateInfo& acInfoRef){
        Dispose();
        bcInfo = bcInfoRef;
        bcInfo.sType = vks::sType(bcInfo);
        acInfo = acInfoRef;

        vmaCreateBuffer(allocator, &bcInfo, &acInfo, &buffer, &allocation, &allocationInfo);
        spdlog::info("Created A buffer of usage {} and size {} in Memory Class \"{}\"", bcInfo.usage, bcInfo.size, name);
    }

    void Memory::SetAsImage(VkImageCreateInfo &icInfoRef, VmaAllocationCreateInfo &acInfoRef) {
        Dispose();
        icInfo = icInfoRef;
        icInfo.sType = vks::sType(icInfo);
        acInfo = acInfoRef;

        vmaCreateImage(allocator, &icInfo, &acInfo, &image, &allocation, &allocationInfo);
        spdlog::info("Created an image of usage {}, format {}, type {}, and extent ({}, {}, {}) in Memory Class \"{}\"", icInfo.usage, icInfo.format, icInfo.imageType, icInfo.extent.width, icInfo.extent.height, icInfo.extent.width, name);
    }

    void Memory::FlushAndInvalidate() {
        vmaFlushAllocation(allocator, allocation, 0, allocationInfo.size);
        vmaInvalidateAllocation(allocator, allocation, 0, allocationInfo.size);
    }

    void Memory::Map() {
        vmaMapMemory(allocator, allocation, &mappedData);
    }
    void Memory::UnMap() {
        vmaUnmapMemory(allocator, allocation);
        mappedData = nullptr;
    }

    ///This happens immediately
    void Memory::TransferFromRam(const void *data, uint64_t srcOffset, uint64_t dstOffset, uint64_t size) {
        assert(size <= bcInfo.size);
        bool wasMapped = mappedData != nullptr;
        if (!wasMapped){
            Map();
        }

        const char* offsetSrc(reinterpret_cast<char*>(const_cast<void*>(data)) + srcOffset);
        char* offsetDst(reinterpret_cast<char*>(const_cast<void*>(mappedData)) + dstOffset);

        memcpy(offsetDst, reinterpret_cast<void const*>(offsetSrc), size);

        if (!wasMapped){
            UnMap();
        }
    }

    void Memory::TransferToBuffer(VkCommandBuffer cmd, Memory &dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size, bool barrier) {
        assert(buffer != nullptr && dst.buffer != nullptr);
        if (barrier){
            VkMemoryBarrier transferBarrier = {};
            transferBarrier.sType = vks::sType(transferBarrier);
            transferBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
            transferBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});
        }
        VkBufferCopy copy = {};
        copy.srcOffset = srcOffset;
        copy.dstOffset = dstOffset;
        copy.size = size;
        vkCmdCopyBuffer(cmd, buffer, dst.buffer, 1, &copy);
        if (barrier){
            VkMemoryBarrier transferBarrier = {};
            transferBarrier.sType = vks::sType(transferBarrier);
            transferBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
            transferBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});
        }
    }

    void Memory::TransferFromBuffer(VkCommandBuffer cmd, Memory &src, uint64_t srcOffset, uint64_t dstOffset, uint64_t size, bool barrier) {
        assert(buffer != nullptr && src.buffer != nullptr);
        if (barrier){
            VkMemoryBarrier transferBarrier = {};
            transferBarrier.sType = vks::sType(transferBarrier);
            transferBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
            transferBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});
        }
        VkBufferCopy copy = {};
        copy.srcOffset = srcOffset;
        copy.dstOffset = dstOffset;
        copy.size = size;
        vkCmdCopyBuffer(cmd, src.buffer, buffer, 1, &copy);
        if (barrier){
            VkMemoryBarrier transferBarrier = {};
            transferBarrier.sType = vks::sType(transferBarrier);
            transferBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
            transferBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});
        }
    }

    void Memory::EnsureCapacity(VkCommandBuffer cmd, TimelineSemaphore &signal, uint64_t size) {
        assert(buffer != nullptr);
        if (size > bcInfo.size) { Resize(cmd, signal, size);}
    }

    void Memory::Resize(VkCommandBuffer cmd, TimelineSemaphore &signal, uint64_t size) {
        VkMemoryBarrier transferBarrier = {};
        transferBarrier.sType = vks::sType(transferBarrier);
        transferBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
        transferBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});

        VkBuffer tempBuffer = nullptr;
        VmaAllocation tempAllocation = nullptr;
        VmaAllocationInfo tempAllocationInfo = {};
        VkBufferCopy copy = {};
        copy.srcOffset = 0;
        copy.dstOffset = 0;
        if (size > bcInfo.size){
            copy.size = bcInfo.size;
        }
        else{
            copy.size = size;
        }
        spdlog::info("Resized buffer {} from used size {} to size {}", name, bcInfo.size, size);
        bcInfo.size = size;

        vmaCreateBuffer(allocator, &bcInfo, &acInfo, &tempBuffer, &tempAllocation, &tempAllocationInfo);
        vkCmdCopyBuffer(cmd, buffer, tempBuffer, 1, &copy);

        std::thread destoryThread([](VmaAllocator allocator, VkBuffer buffer, VmaAllocation allocation, TimelineSemaphore* cleanupSignal){
            cleanupSignal->Wait();
            vmaDestroyBuffer(allocator, buffer, allocation);
            spdlog::info("Cleanup activated");
            }, allocator, buffer, allocation, &signal);
        destoryThread.detach();


        buffer = tempBuffer;
        allocation = tempAllocation;
        allocationInfo = tempAllocationInfo;

        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});

    }

    uint64_t Memory::SizeInUse() {
        assert((buffer != nullptr || image != nullptr) && !(buffer != nullptr && image != nullptr));
        if (buffer != nullptr){
            return bcInfo.size;
        }
        else{
            VkMemoryRequirements reqs;
            vkGetImageMemoryRequirements(device, image, &reqs);
            return reqs.size;
        }

    }

    void Memory::AttachBufferBinding(DescriptorSet &targetSet, uint64_t offset, uint64_t range) {
        assert(buffer != nullptr && offset <= bcInfo.size);

        if (range == 0){
            range = bcInfo.size - offset;
        }
        VkDescriptorBufferInfo bInfo = {};
        bInfo.buffer = buffer;
        bInfo.offset = 0;
        bInfo.range = range;
        VkDescriptorType type;

        if  (bcInfo.usage == VK_BUFFER_USAGE_STORAGE_BUFFER_BIT){
            type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        } else{
            assert(false);
        }

        DescriptorBinding binding(type, 1, VK_SHADER_STAGE_VERTEX_BIT, true, false, false, bInfo, {},
                                  {});

        targetSet.AddBinding(binding);
    }

    void Memory::AttachImageBinding(DescriptorSet &targetSet, VkImageView view, VkImageLayout layout, VkSampler sampler) {
        assert(image != nullptr);
        VkDescriptorType type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        VkDescriptorImageInfo iInfo = {};
        iInfo.imageView = view;
        iInfo.imageLayout = layout;
        iInfo.sampler = sampler;
        DescriptorBinding binding(type, 1, VK_SHADER_STAGE_VERTEX_BIT, true, false, false, {}, iInfo,
                {});

        targetSet.AddBinding(binding);
    }

    void Memory::TransferBufferToImage(VkCommandBuffer cmd, Memory &dst, VkImageLayout currentLayout, std::vector<VkBufferImageCopy> regions) {
        assert(dst.image != nullptr);
        vkCmdCopyBufferToImage(cmd, buffer, dst.image, currentLayout, regions.size(), regions.data());
    }
}