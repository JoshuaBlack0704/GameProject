#pragma once
#include <vector>
#include <functional>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../VkBootstrap/VkBootstrap.h"

#define VK_LAYER_NAMESPACE vkl

namespace VK_LAYER_NAMESPACE
{
    //Static variables where only one should be in existence at a time
    extern GLFWwindow* window;
    extern vkb::Instance instance;
    extern vkb::PhysicalDevice pDevice;
    extern vkb::Device lDevice;
    extern vkb::Swapchain swapchain;
    extern VkSurfaceKHR surface;
    extern std::vector<std::function<void(GLFWwindow* window, int width, int height)>> resizeCallbacks;

    //Static settings
    extern int windowWidth;
    extern int windowHeight;
    extern bool useImGui;


    ///This function contains arguments for all the variable settings
    void SetSettings(int windowWidth, int windowHeight, bool useImGui, bool resizable);
    ///This creates all of the top level vulkan objects as well as calls whatever functions are
    ///In the resizeCallbacks vector
    void InitVkLayer();
    void AddResizeCallBack(std::function<void(GLFWwindow *window, int width, int height)> callback);


}

