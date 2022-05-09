#include <vksMemory.h>
#include <vksTypes.h>
#include <spdlog/spdlog.h>

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
}