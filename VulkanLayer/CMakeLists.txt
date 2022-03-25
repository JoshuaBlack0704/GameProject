file(GLOB_RECURSE src "*.cpp" "*.c")
file(GLOB_RECURSE incl "*.hpp" "*.h")

add_library(vkLayer ${src} ${incl} "Include/VkLayer.hpp")

target_include_directories(vkLayer PRIVATE ${VULKAN_INCLUDE_DIR})
target_link_libraries(vkLayer PRIVATE spdlog::spdlog VulkanMemoryAllocator glm glfw -ldl)
target_include_directories(vkLayer INTERFACE "Include/")