#pragma once
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>



namespace vks
{
    class WindowCallbackHandler {
    public:
        explicit WindowCallbackHandler(GLFWwindow* window);
        void AddResizeCallback(std::function<void(GLFWwindow*, int width, int height)> callback);
        void AddResizeCallback(std::function<void()> callback);
        void AddKeyCallback(std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> callback);
        void AddKeyCallback(std::function<void()> callback);

    private:
        static void RunResizeCallbacks(GLFWwindow*, int width, int height);
        static void RunKeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
        GLFWwindow* window;

        std::vector<std::function<void(GLFWwindow*, int width, int height)>> resizeCallbacks;
        std::vector<std::function<void()>> emptyResizeCallbacks;

        std::vector<std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>> keyCallbacks;
        std::vector<std::function<void()>> emptyKeyCallbacks;

    };
}



