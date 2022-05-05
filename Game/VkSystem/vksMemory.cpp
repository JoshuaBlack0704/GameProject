#include <vksMemory.h>
#include <vksTypes.h>
#include <spdlog/spdlog.h>

namespace vks
{
    Memory::Memory(VmaAllocator allocator) : allocator(allocator) {}

    void Memory::Dispose() {
        if (buffer != nullptr){
            vmaDestroyBuffer(allocator, buffer, allocation);
            allocation = nullptr;
            buffer = nullptr;

        }
        if (image != nullptr){
            vmaDestroyImage(allocator, image, allocation);
            allocation = nullptr;
            image = nullptr;
        }
    }

    void Memory::SetAsBuffer(VkBufferCreateInfo &bcInfoRef, VmaAllocationCreateInfo& acInfoRef){
        Dispose();
        bcInfo = bcInfoRef;
        bcInfo.sType = vks::sType(bcInfo);
        acInfo = acInfoRef;

        vmaCreateBuffer(allocator, &bcInfo, &acInfo, &buffer, &allocation, &allocationInfo);
        spdlog::info("Created A buffer of usage {} and size {}", bcInfo.usage, bcInfo.size);
    }

    void Memory::SetAsImage(VkImageCreateInfo &icInfoRef, VmaAllocationCreateInfo &acInfoRef) {
        Dispose();
        icInfo = icInfoRef;
        icInfo.sType = vks::sType(icInfo);
        acInfo = acInfoRef;

        vmaCreateImage(allocator, &icInfo, &acInfo, &image, &allocation, &allocationInfo);
        spdlog::info("Created an image of usage {}, format {}, type {}, and extent ({}, {}, {})", icInfo.usage, icInfo.format, icInfo.imageType, icInfo.extent.width, icInfo.extent.height, icInfo.extent.width);
    }
}