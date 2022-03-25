file(GLOB_RECURSE src "*.cpp" "*.c")
file(GLOB_RECURSE incl "*.hpp" "*.h")

add_library(vkLayer ${src} ${incl})

target_include_directories(vkLayer PRIVATE ${VULKAN_INCLUDE_DIR})
target_link_libraries(vkLayer PRIVATE glfw spdlog::spdlog VulkanMemoryAllocator glm)
target_include_directories(vkLayer INTERFACE "Include/")