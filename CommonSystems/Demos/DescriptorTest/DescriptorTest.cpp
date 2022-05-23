#include <spdlog/spdlog.h>
#include <VkSystem.h>
#include <cmnCommon.h>

int main(){
    spdlog::info("Starting Descriptor Test");
    vks::VkData vkData = vks::VkData(false);

    {
        vks::Memory testStorage = vks::Memory(vkData.lDevice.device, vkData.allocator, "Test");
        VkBufferCreateInfo bcInfo = {};
        bcInfo.sType = vks::sType(bcInfo);
        bcInfo.size = 100000;
        bcInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        VmaAllocationCreateInfo acInfo = {};
        acInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        testStorage.SetAsBuffer(bcInfo, acInfo);

        vks::DescriptorPool pool(vkData.lDevice.device);
        vks::DescriptorSet set(vkData.lDevice.device);

        testStorage.AttachBufferBinding(set);

        set.Allocate(pool);

        set.Write();
    }


}