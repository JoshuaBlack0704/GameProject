//
// Created by josh on 3/23/22.
//
#define USESPDLOG

#include <spdlog/spdlog.h>
#include <VkSystem.h>
#include <flecs.h>
#include <imgui.h>
#include <FileIO.h>

int main(){

    vks::vksVkData vkData;
    {
        VkCommandPoolCreateInfo cInfo = {};
        cInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::graphics).value();
        vks::CmdPoolWrapper cmdPoolWrapper(vkData.lDevice.device, cInfo);
        vks::CmdBufferSet& cmdSet = cmdPoolWrapper.MakeSet();
        cmdSet.AddCmdBuffers(1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        vks::ShaderStore shaderStore(vkData.lDevice.device);
        auto vertexShader = shaderStore.AddShader("simpletrianglevert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
        auto fragmentShader = shaderStore.AddShader("simpletrianglefrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
        shaderStore.ReloadShaders();

        vks::WindowCallbackHandler callbackHandler(vkData.window);
        callbackHandler.AddResizeCallback([](GLFWwindow*, int width, int height){spdlog::info("Resizeing window to: {} x {}", width, height);});
        callbackHandler.AddResizeCallback(vkData.resizeCallback);
        callbackHandler.AddKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mods){spdlog::info("Pressing key of code: {}, with action: {}", scancode, action);});


        spdlog::info("Running");

        while (!glfwWindowShouldClose(vkData.window))
        {
            glfwPollEvents();
        }
    }

}
