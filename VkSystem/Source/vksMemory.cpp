#include <vksMemory.h>
#include <vksTypes.h>
#include <spdlog/spdlog.h>
#include <assert.h>
#include <thread>

namespace vks
{
    Memory::Memory(VmaAllocator allocator, std::string name) : allocator(allocator), name(name) {}

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
        spdlog::info("Created A buffer of usage {} and size {} in Memory Class \"{}\"", bcInfo.usage, allocationInfo.size, name);
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
    void Memory::TransferFromRam(const void *data, uint64_t srcOffset, uint64_t size, uint64_t dstOffset) {
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

    void Memory::TransferToBuffer(VkCommandBuffer cmd, Memory &dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size) {
        assert(buffer != nullptr && dst.buffer != nullptr);
        VkBufferCopy copy = {};
        copy.srcOffset = srcOffset;
        copy.dstOffset = dstOffset;
        copy.size = size;
        vkCmdCopyBuffer(cmd, buffer, dst.buffer, 1, &copy);
    }

    void Memory::TransferFromBuffer(VkCommandBuffer cmd, Memory &src, uint64_t srcOffset, uint64_t dstOffset, uint64_t size) {
        assert(buffer != nullptr && src.buffer != nullptr);
        VkBufferCopy copy = {};
        copy.srcOffset = srcOffset;
        copy.dstOffset = dstOffset;
        copy.size = size;
        vkCmdCopyBuffer(cmd, src.buffer, buffer, 1, &copy);
    }

    void Memory::EnsureCapacity(VkCommandBuffer cmd, VkSemaphore cleanupSignal, uint64_t size) {
        assert(buffer != nullptr);
        if (size > allocationInfo.size) { Resize(cmd, cleanupSignal, size);}
    }

    void Memory::Resize(VkCommandBuffer cmd, VkSemaphore cleanupSignal, uint64_t size) {
        VkMemoryBarrier transferBarrier = {};
        transferBarrier.sType = vks::sType(transferBarrier);
        transferBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
        transferBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});

        VkBuffer tempBuffer = nullptr;
        VmaAllocation tempAllocation = nullptr;
        VmaAllocationInfo tempAllocationInfo = {};
        bcInfo.size = size;

        vmaCreateBuffer(allocator, &bcInfo, &acInfo, &tempBuffer, &tempAllocation, &tempAllocationInfo);
        VkBufferCopy copy = {};
        copy.srcOffset = 0;
        copy.dstOffset = 0;
        if (size > allocationInfo.size){
            copy.size = allocationInfo.size;
        }
        else{
            copy.size = size;
        }

        vkCmdCopyBuffer(cmd, buffer, tempBuffer, 1, &copy);

        std::thread destoryThread([](VkBuffer buffer, VmaAllocation allocation, VkSemaphore cleanupSignal){
            wait
            }, buffer, allocation, cleanupSignal);

        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, 1, &transferBarrier, 0, {}, 0, {});

    }
}