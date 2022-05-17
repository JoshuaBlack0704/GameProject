#include <spdlog/spdlog.h>
#include <VkSystem.h>
#include <cmnGLFWSystem.h>

int main(){
    spdlog::info("Starting GLFW System Test");
    vks::VkData vkData = vks::VkData(true);

    cmn::GLFWSystem glfwSystem = cmn::GLFWSystem(vkData.window, vkData.surface);
    glfwSystem.AddKeyCallback([](GLFWwindow*,int key,int,int action,int){
        if (key == GLFW_KEY_0){
            if (action == GLFW_PRESS){
                spdlog::info("Pressed Key 0");
            }
            else if (action == GLFW_RELEASE){
                spdlog::info("Released Key 0");
            }
        }
    });
    glfwSystem.AddCursorPositionCallback([](GLFWwindow* window, double xpos, double ypos){
        int totalWidth = 0;
        int totalHeight = 0;

        glfwGetWindowSize(window, &totalWidth, &totalHeight);
        spdlog::info("Cursor is at ({}, {}) px : ({:.3f}, {:.3f}) screen-space", xpos, ypos, xpos/totalWidth, ypos/totalHeight);
    });

    while(!glfwWindowShouldClose(vkData.window)){
        glfwPollEvents();
    }


    spdlog::info("Ending GLFW System Test");
}