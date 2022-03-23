#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main(int, char**) {
    glfwInit();
    auto window = glfwCreateWindow(100,100,"Title", nullptr, nullptr);
    std::cout << "Hello, world!\n";

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}
