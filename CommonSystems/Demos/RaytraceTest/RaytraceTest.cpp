#include <VkSystem.h>
#include <cmnCommon.h>
#include <spdlog/spdlog.h>
#include <flecs.h>
#include <functional>

int main(){
    spdlog::info("Starting Raytrace test");
    vks::VkData vkData = vks::VkData(true);

    {
        cmn::GLFWSystem glfwSystem = cmn::GLFWSystem(vkData);
        vks::Memory rayimageMemory = vks::Memory(vkData.lDevice.device, vkData.allocator, "Ray Image Memory");
        std::vector<uint32_t> data;
        auto memoryCreate = [&vkData, &rayimageMemory, &data](GLFWwindow*,int width,int height){
            VkBufferCreateInfo bcInfo = {};
            bcInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bcInfo.size = sizeof (uint32_t) * vkData.swapchain.extent.width * vkData.swapchain.extent.height;
            data.clear();
            data.resize(vkData.swapchain.extent.width*vkData.swapchain.extent.height, 1);
            VmaAllocationCreateInfo acInfo = {};
            acInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            rayimageMemory.SetAsBuffer(bcInfo, acInfo);

            spdlog::info("Resized ray image to {} bytes", bcInfo.size);
        };
        memoryCreate(nullptr,0,0);
        glfwSystem.AddResizeCallback(memoryCreate);

        VkCommandPoolCreateInfo cInfo = {};
        cInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::transfer).value();
        vks::CmdPoolWrapper cmdPool(vkData.lDevice.device, cInfo, "Main Command Pool");

        vks::CmdBufferSet transferSet = cmdPool.MakeSet("Transfer Set");


        vks::TimelineSemaphore timelineSemaphore(vkData.lDevice.device, 0);
        vks::Semaphore semaphore(vkData.lDevice.device);
        std::vector<VkSemaphore> waitSemaphores = {semaphore.semaphore, timelineSemaphore.GetSemaphore()};


        while (!glfwWindowShouldClose(vkData.window)){
            glfwPollEvents();
            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(vkData.lDevice.device, vkData.swapchain.swapchain, UINT64_MAX, semaphore.semaphore, nullptr, &imageIndex);

            rayimageMemory.TransferFromRam(data.data(), 0, 0, rayimageMemory.SizeInUse());
            transferSet.SetFunctions({
                                             [&rayimageMemory, &data, &vkData, &imageIndex](VkCommandBuffer cmd){
                                                 VkCommandBufferBeginInfo beginInfo = {};
                                                 vkBeginCommandBuffer(cmd,&beginInfo);
                                                 VkBufferImageCopy copy = {};
                                                 VkImageSubresourceLayers layers = {};
                                                 layers.aspectMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                                                 layers.baseArrayLayer = 0; layers.mipLevel = 0; layers.layerCount = 1;
                                                 copy.imageSubresource = layers;
                                                 copy.imageOffset = {0, 0};
                                                 copy.imageExtent = VkExtent3D{vkData.swapchain.extent.width, vkData.swapchain.extent.width, 1};
                                                 vkCmdCopyBufferToImage(cmd, rayimageMemory.buffer, vkData.swapchain.get_images().value()[imageIndex], VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, &copy);



                                             }
                                     });
            transferSet.Record();

            auto tSubmit = timelineSemaphore.GetSubmitInfo(true, {}, {semaphore});
            VkSubmitInfo submit = {};
            submit.sType = vks::sType(submit);
            submit.pNext = &tSubmit;
            submit.waitSemaphoreCount = ;
            submit.pWaitSemaphores =



        }
    }

}

