#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"
#include "VkBootstrap.h"

struct MainData{

    GLFWwindow* window = nullptr;
    vkb::Instance instance;
    vk::SurfaceKHR surface;
    vkb::PhysicalDevice pDevice;
    vkb::Device device;
    VmaAllocator allocator = nullptr;
};

struct ImGuiInfo
{

};