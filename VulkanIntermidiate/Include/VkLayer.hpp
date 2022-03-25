#pragma once
#include <vector>
#include <functional>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>
#include "../VkBootstrap/VkBootstrap.h"

#define VK_LAYER_NAMESPACE vkl

namespace VK_LAYER_NAMESPACE
{
    //Static variables where only one should be in existence at a time
    GLFWwindow* window;
    vkb::Instance instance;
    vkb::PhysicalDevice pDevice;
    vkb::Device lDevice;
    vkb::Swapchain swapchain;
    VkSurfaceKHR surface;
    std::vector<std::function<void(GLFWwindow* window, int width, int height)>> resizeCallbacks;

    //Static settings
    int windowWidth = 100;
    int windowHeight = 100;
    bool useImGui = true;
#ifndef NDEBUG
    bool useValidationLayers = true;
#else
    bool useValidationLayers = false;
#endif
    bool resizeable = false;

    void SetSettings(int windowWidth, int windowHeight, bool useImGui, bool resizable);
    void InitVkLayer();


}

