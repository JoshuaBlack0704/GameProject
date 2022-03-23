#include "../Include/API.hpp"
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

void Logger::Log(){
    spdlog::info("hello world");
    glfwInit();
    auto window = glfwCreateWindow(100, 100, "title", nullptr, nullptr);
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}
