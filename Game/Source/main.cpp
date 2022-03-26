//
// Created by josh on 3/23/22.
//
#include <VkLayer.hpp>
#include <spdlog/spdlog.h>

int main(){



    auto h = [](GLFWwindow*, int, int){spdlog::info("Resizing");};
    vkl::AddResizeCallBack(h);
    vkl::InitVkLayer();
}
