#include <cmnGLFWSystem.h>
#include <spdlog/spdlog.h>

namespace cmn{
    GLFWSystem::GLFWSystem(vks::VkData &vkData) : window(vkData.window), surface(vkData.surface) {
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height){
            spdlog::info("Window resized to {} x {}", width, height);
            GLFWSystem* glfwSystem = reinterpret_cast<GLFWSystem*>(glfwGetWindowUserPointer(window));
           glfwSystem->ExecuteResizeCallbacks(window, width, height);
        });
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            spdlog::info("Executing key callbacks (key, scancode, action, mods): ({}, {}, {}, {}): ", key, scancode, action, mods);
            GLFWSystem* glfwSystem = reinterpret_cast<GLFWSystem*>(glfwGetWindowUserPointer(window));
            glfwSystem->ExecuteKeyCallbacks(window, key, scancode, action, mods);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
            GLFWSystem* glfwSystem = reinterpret_cast<GLFWSystem*>(glfwGetWindowUserPointer(window));
            glfwSystem->ExecuteCursorPositionCallbacks(window, xpos, ypos);
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods){
            spdlog::info("Executing mouse button callbacks (button, action, mods): ({}, {}, {}): ", button, action, mods);
            GLFWSystem* glfwSystem = reinterpret_cast<GLFWSystem*>(glfwGetWindowUserPointer(window));
            glfwSystem->ExecuteMouseButtonCallbacks(window, button, action, mods);
        });


        AddResizeCallback([&vkData](GLFWwindow * window, int width, int height){
            spdlog::info("Rebuilding Swapchain");

            auto swapRet = vkb::SwapchainBuilder(vkData.lDevice)
                    .set_old_swapchain(vkData.swapchain.swapchain)
                    .set_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                    .build();

            vkData.swapchain = swapRet.value();

            if (!swapRet){
                spdlog::error("Could not create swapchain");
                abort();
            }
        });
    }

    void GLFWSystem::AddResizeCallback(std::function<void(GLFWwindow * window, int width, int height)> callback) {
        resizeCallbacks.emplace_back(callback);
    }

    void GLFWSystem::ExecuteResizeCallbacks(GLFWwindow *window, int width, int height) {
        for (auto& callback : resizeCallbacks){
            callback(window, width, height);
        }
    }

    void GLFWSystem::AddKeyCallback(std::function<void(GLFWwindow *, int, int, int, int)> callback) {
        keyCallbacks.emplace_back(callback);
    }

    void GLFWSystem::ExecuteKeyCallbacks(GLFWwindow *window, int key, int scancode, int action, int mods) {
        for (auto& callback : keyCallbacks){
            callback(window, key, scancode, action, mods);
        }
    }

    void GLFWSystem::AddCursorPositionCallback(std::function<void(GLFWwindow *, double, double)> callback) {
        cursorPosCallbacks.emplace_back(callback);
    }

    void GLFWSystem::ExecuteCursorPositionCallbacks(GLFWwindow *window, double xpos, double ypos) {
        for (auto& callback : cursorPosCallbacks){
            callback(window, xpos, ypos);
        }
    }

    void GLFWSystem::AddMouseButtonCallback(std::function<void(GLFWwindow *, int, int, int)> callback) {
        mouseButtonCallbacks.emplace_back(callback);
    }

    void GLFWSystem::ExecuteMouseButtonCallbacks(GLFWwindow *window, int button, int action, int mods) {
        for (auto& callback : mouseButtonCallbacks){
            callback(window, button, action, mods);
        }
    }
}