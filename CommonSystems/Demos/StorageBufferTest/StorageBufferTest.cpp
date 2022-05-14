#include <vector>
#include <spdlog/spdlog.h>
#include <VkSystem.h>

int main(){
    vks::VkData vkData = vks::VkData();
    {
        spdlog::info("Starting Storage Buffer Test");
        std::string message = "Hello From GPU";

        VkBufferCreateInfo bcInfo = {};
        bcInfo.size = message.size();
        bcInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VmaAllocationCreateInfo acInfo = {};
        acInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        vks::Memory stageBuffer(vkData.allocator, "Storage Buffer");
        stageBuffer.SetAsBuffer(bcInfo, acInfo);
        stageBuffer.TransferFromRam(message.data(), 0, message.size(), 0);

        bcInfo.size = stageBuffer.allocationInfo.size;
        acInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        vks::Memory gpuStorage(vkData.allocator, "GPU Storage");
        gpuStorage.SetAsBuffer(bcInfo, acInfo);

        bcInfo.size = stageBuffer.allocationInfo.size;
        acInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        vks::Memory receiveBuffer(vkData.allocator, "Receive Buffer");
        receiveBuffer.SetAsBuffer(bcInfo, acInfo);

        vks::TimelineSemaphore signal(vkData.lDevice.device, 0);

        VkCommandPoolCreateInfo cInfo = {};
        cInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::transfer).value();
        vks::CmdPoolWrapper cmdPool(vkData.lDevice.device, cInfo, "Main Command Pool");

        auto& sendSet = cmdPool.MakeSet("Main Command Set");
        sendSet.AddCmdBuffers(1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        sendSet.SetFunctions({[&stageBuffer, &gpuStorage, &receiveBuffer, &signal](VkCommandBuffer cmd){


            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = vks::sType(beginInfo);

            vkBeginCommandBuffer(cmd, &beginInfo);

            gpuStorage.EnsureCapacity(cmd,  signal, stageBuffer.allocationInfo.size);
            receiveBuffer.EnsureCapacity(cmd, signal, stageBuffer.allocationInfo.size);
            stageBuffer.TransferToBuffer(cmd, gpuStorage, 0, 0, stageBuffer.allocationInfo.size, true);
            receiveBuffer.TransferFromBuffer(cmd, gpuStorage, 0, 0, gpuStorage.allocationInfo.size, false);

            vkEndCommandBuffer(cmd);
        }});

        auto transferQueue = vkData.lDevice.get_queue(vkb::QueueType::transfer).value();

        auto timelineInfo = signal.GetSubmitInfo(true, {});
        VkSubmitInfo submit = {};
        submit.sType = vks::sType(submit);
        submit.pNext = &timelineInfo;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = sendSet.pCommandBuffers();
        submit.waitSemaphoreCount = 0;
        submit.pWaitSemaphores = nullptr;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &signal.GetSemaphore();

        sendSet.Record();
        vkQueueSubmit(transferQueue, 1, &submit, VK_NULL_HANDLE);

        signal.Wait();


        receiveBuffer.FlushAndInvalidate();

        receiveBuffer.Map();

        std::string returnedMessage = std::string (reinterpret_cast<char*>(receiveBuffer.mappedData), message.size());

        receiveBuffer.UnMap();

        spdlog::info("Returned Message \"{}\"", returnedMessage);

        spdlog::info("Ending Storage Buffer Test");
    }

}

