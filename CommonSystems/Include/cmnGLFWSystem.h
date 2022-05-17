#pragma once
#include <vector>
#include <functional>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"

namespace cmn{

    class GLFWSystem{
    public:
        GLFWSystem(GLFWwindow *window, VkSurfaceKHR surface);
        void AddResizeCallback(std::function<void(GLFWwindow* window, int width, int height)> callback);
        void ExecuteResizeCallbacks(GLFWwindow *window, int width, int height);
        void AddKeyCallback(std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> callback);
        void ExecuteKeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
        void AddCursorPositionCallback(std::function<void(GLFWwindow* window, double xpos, double ypos)> callback);
        void ExecuteCursorPositionCallbacks(GLFWwindow* window, double xpos, double ypos);
        void AddMouseButtonCallback(std::function<void(GLFWwindow* window, int button, int action, int mods)> callback);
        void ExecuteMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
    private:
        GLFWwindow*  window = nullptr;
        VkSurfaceKHR surface = nullptr;
        std::vector<std::function<void(GLFWwindow* window, int width, int height)>> resizeCallbacks;
        std::vector<std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>> keyCallbacks;
        std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> cursorPosCallbacks;
        std::vector<std::function<void(GLFWwindow* window, int button, int action, int mods)>> mouseButtonCallbacks;

    };
}