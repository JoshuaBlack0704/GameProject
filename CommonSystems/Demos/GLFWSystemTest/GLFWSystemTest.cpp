#include <spdlog/spdlog.h>
#include <VkSystem.h>
#include <cmnGLFWSystem.h>

int main(){
    spdlog::info("Starting GLFW System Test");
    vks::VkData vkData = vks::VkData(true);

    while(!glfwWindowShouldClose(vkData.window)){
        glfwPollEvents();
    }


    spdlog::info("Ending GLFW System Test");
}