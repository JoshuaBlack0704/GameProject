#include <VkSystem.h>
#include <cmnCommon.h>
#include <spdlog/spdlog.h>
#include <flecs.h>
#include <functional>
#include <glm/glm.hpp>
#include <taskflow.hpp>
#include <bitset>


int main(){
    spdlog::info("Starting Raytrace test");
    vks::VkData vkData = vks::VkData(true);

    {
        cmn::GLFWSystem glfwSystem = cmn::GLFWSystem(vkData);

        VkCommandPoolCreateInfo cInfo = {};
        cInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::graphics).value();
        vks::CmdPoolWrapper cmdPool(vkData.lDevice.device, cInfo, "Main Command Pool");

        vks::CmdBufferSet& cmdSet = cmdPool.MakeSet("Command Set");

        vks::Fence fence(vkData.lDevice.device, VK_FENCE_CREATE_SIGNALED_BIT);
        vks::Semaphore acquireImageSemaphore(vkData.lDevice.device);
        vks::Semaphore transferDoneSemaphore(vkData.lDevice.device);
        std::vector<VkSemaphore> waitSemaphores = {acquireImageSemaphore.semaphore};
        std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
        uint32_t imageIndex = 0;
        cmdSet.SetFunctions({
                                         [](VkCommandBuffer cmd){
                                         }
                                 });


        while (!glfwWindowShouldClose(vkData.window)){
            glfwPollEvents();
            fence.Reset();
            vkAcquireNextImageKHR(vkData.lDevice.device, vkData.swapchain.swapchain, UINT64_MAX, acquireImageSemaphore.semaphore, nullptr, &imageIndex);

            cmdPool.Reset(true);

            VkSubmitInfo submit = {};
            submit.sType = vks::sType(submit);
            submit.waitSemaphoreCount = waitSemaphores.size();
            submit.pWaitSemaphores = waitSemaphores.data();
            submit.pWaitDstStageMask = waitStages.data();
            submit.commandBufferCount = 1;
            submit.pCommandBuffers = cmdSet.pCommandBuffers();
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores = &transferDoneSemaphore.semaphore;


            VkPresentInfoKHR presentInfoKhr = {};
            presentInfoKhr.sType = vks::sType(presentInfoKhr);
            presentInfoKhr.waitSemaphoreCount = 1;
            presentInfoKhr.pWaitSemaphores = &transferDoneSemaphore.semaphore;
            presentInfoKhr.swapchainCount = 1;
            presentInfoKhr.pSwapchains = &vkData.swapchain.swapchain;
            presentInfoKhr.pImageIndices = &imageIndex;
            VkResult result;
            presentInfoKhr.pResults = &result;

            vkQueueSubmit(vkData.lDevice.get_queue(vkb::QueueType::graphics).value(), 1, &submit, fence.GetFence());

            vkQueuePresentKHR(vkData.lDevice.get_queue(vkb::QueueType::graphics).value(), &presentInfoKhr);
            fence.Wait();

        }
    }

}

