#include <vector>
#include <spdlog/spdlog.h>
#include <VkSystem.h>
#include <sstream>
#include <thread>

int main(){
    vks::VkData vkData = vks::VkData(false);
    {
        spdlog::info("Starting Storage Buffer Test");
        std::string message = "Hello from GPU number: 0";
        std::string newMessage = message;

        VkBufferCreateInfo stageBCInfo = {};
        stageBCInfo.sType = vks::sType(stageBCInfo);
        stageBCInfo.size = message.size();
        stageBCInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VkBufferCreateInfo gpuBCInfo = {};
        gpuBCInfo.sType = vks::sType(stageBCInfo);
        gpuBCInfo.size = message.size();
        gpuBCInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VkBufferCreateInfo receiveBCInfo = {};
        receiveBCInfo.sType = vks::sType(stageBCInfo);
        receiveBCInfo.size = message.size();
        receiveBCInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VmaAllocationCreateInfo acInfo = {};
        acInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        vks::Memory stageBuffer(nullptr, vkData.allocator, "Stage Buffer");
        stageBuffer.SetAsBuffer(stageBCInfo, acInfo);

        acInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        vks::Memory gpuStorage(nullptr, vkData.allocator, "GPU Storage");
        gpuStorage.SetAsBuffer(gpuBCInfo, acInfo);

        acInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        vks::Memory receiveBuffer(nullptr, vkData.allocator, "Receive Buffer");
        receiveBuffer.SetAsBuffer(receiveBCInfo, acInfo);

        vks::TimelineSemaphore signal(vkData.lDevice.device, 0);

        VkCommandPoolCreateInfo cInfo = {};
        cInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::transfer).value();
        vks::CmdPoolWrapper cmdPool(vkData.lDevice.device, cInfo, "Main Command Pool");

        auto& sendSet = cmdPool.MakeSet("Main Command Set");
        sendSet.SetFunctions({[&stageBuffer, &gpuStorage, &receiveBuffer, &signal, &message, &newMessage](VkCommandBuffer cmd){

            stageBuffer.TransferFromRam(message.data(), 0, 0, message.size());

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = vks::sType(beginInfo);

            vkBeginCommandBuffer(cmd, &beginInfo);

            gpuStorage.EnsureCapacity(cmd,  signal, stageBuffer.SizeInUse());
            receiveBuffer.EnsureCapacity(cmd, signal, stageBuffer.SizeInUse());
            stageBuffer.TransferToBuffer(cmd, gpuStorage, 0, 0, stageBuffer.SizeInUse(), true);
            receiveBuffer.TransferFromBuffer(cmd, gpuStorage, 0, 0, gpuStorage.SizeInUse(), false);

            stageBuffer.EnsureCapacity(cmd, signal, newMessage.size());

            vkEndCommandBuffer(cmd);
        }});

        auto transferQueue = vkData.lDevice.get_queue(vkb::QueueType::transfer).value();



        for (int i = 0; i < 150; ++i) {
            std::stringstream messageStream;
            messageStream << "Hello from GPU number: " << std::to_string(i);
            newMessage = messageStream.str();

            auto timelineInfo = signal.GetSubmitInfo(true, {}, {});
            VkSubmitInfo submit = {};
            submit.sType = vks::sType(submit);
            submit.pNext = &timelineInfo;
            submit.commandBufferCount = 1;
            submit.pCommandBuffers = sendSet.pCommandBuffers();
            submit.waitSemaphoreCount = 0;
            submit.pWaitSemaphores = nullptr;
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores = &signal.GetSemaphore();

            cmdPool.Reset(true);
            vkQueueSubmit(transferQueue, 1, &submit, VK_NULL_HANDLE);

            signal.Wait();


            receiveBuffer.FlushAndInvalidate();

            receiveBuffer.Map();

            std::string returnedMessage = std::string (reinterpret_cast<char*>(receiveBuffer.mappedData), message.size());

            receiveBuffer.UnMap();

            std::thread log([](std::string message){spdlog::info("Returned Message \"{}\"", message);}, returnedMessage);
            log.detach();

            message = std::string (newMessage);

        }
        spdlog::info("Ending Storage Buffer Test");
    }

}

