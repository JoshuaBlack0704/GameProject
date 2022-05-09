#include "vksGLFWCallbackHandler.h"



vks::WindowCallbackHandler::WindowCallbackHandler(GLFWwindow *window) : window(window){
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, vks::WindowCallbackHandler::RunResizeCallbacks);
    glfwSetKeyCallback(window, vks::WindowCallbackHandler::RunKeyCallbacks);
}
void vks::WindowCallbackHandler::AddResizeCallback(std::function<void(GLFWwindow*, int width, int height)> callback){
    resizeCallbacks.emplace_back(callback);
}
void vks::WindowCallbackHandler::AddResizeCallback(std::function<void()> callback){
    emptyResizeCallbacks.emplace_back(callback);
}
void vks::WindowCallbackHandler::AddKeyCallback(std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> callback){
    keyCallbacks.emplace_back(callback);
}
void vks::WindowCallbackHandler::AddKeyCallback(std::function<void()> callback){
    emptyKeyCallbacks.emplace_back(callback);
}

void vks::WindowCallbackHandler::RunResizeCallbacks(GLFWwindow* window, int width, int height){
    WindowCallbackHandler* handler = reinterpret_cast<WindowCallbackHandler*>(glfwGetWindowUserPointer(window));
    for (auto& callback : handler->resizeCallbacks){
        callback(window, width, height);
    }
    for (auto& callback : handler->emptyResizeCallbacks){
        callback();
    }
}
void vks::WindowCallbackHandler::RunKeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods){
    WindowCallbackHandler* handler = reinterpret_cast<WindowCallbackHandler*>(glfwGetWindowUserPointer(window));
    for (auto& callback : handler->keyCallbacks){
        callback(window, key, scancode, action, mods);
    }
    for (auto& callback : handler->emptyKeyCallbacks){
        callback();
    }
}