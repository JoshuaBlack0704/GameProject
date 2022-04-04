//
// Created by josh on 3/23/22.
//
#include <spdlog/spdlog.h>
#include <VkSystem.h>
#include <flecs.h>
#include <imgui.h>

int main(){

    flecs::world world;

    flecs::entity vkSystem = world.entity("vkSystem");
    vkSystem.add<vks::VkSystemData>();

    vkSystem.set<vks::ResizeCallback>({[&world](GLFWwindow* window, int width, int height, flecs::entity e){
        auto data = e.get<vks::VkSystemData>();
        auto oldChain = data->swapchain;
        auto swapRet = vkb::SwapchainBuilder(data->lDevice)
                .set_old_swapchain(data->swapchain)
                .build();
        if (!swapRet){
            spdlog::error("During Resize, could not recreate swapchain");
            abort();
        }
        vkb::destroy_swapchain(data->swapchain);
        vks::VkSystemData newData {data->window,
                                   data->surface,
                                   data->instance,
                                   data->pDevice,
                                   data->lDevice,
                                   swapRet.value(),
                                   &world,
                                   data->allocator};
        world.each<vks::VkSystemData>([&newData](vks::VkSystemData& data){data = newData;});
        spdlog::info("Resizing to {} x {}", width, height);
    }});

    //We should only remove a VkSystemData if we want the vulkan instance and all of its children destroyed
    world.trigger<vks::VkSystemData>("OnRemoveData")
            .event(flecs::OnRemove)
            .each([](vks::VkSystemData& data){
                vmaDestroyAllocator(data.allocator);
                vkDestroySwapchainKHR(data.lDevice.device, data.swapchain.swapchain, nullptr);
                vkDestroyDevice(data.lDevice.device, nullptr);
                vkDestroySurfaceKHR(data.instance.instance, data.surface, nullptr);
                vkDestroyInstance(data.instance.instance, nullptr);
                spdlog::info("GoodBye!");
            });
    world.import<vks::VkModule>();
    world.progress();


    spdlog::info("Running");

    while (!glfwWindowShouldClose(vkSystem.get<vks::VkSystemData>()->window))
    {
        world.progress();
        glfwPollEvents();
    }


    vkSystem.remove<vks::VkSystemData>();
}
