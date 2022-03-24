file(GLOB src "Source/*.cpp" "ImGui/*.cpp")
file(GLOB incl "Include/*.hpp" "ImGui/*.hpp")

add_library(vil ${src} ${incl})

target_include_directories(vil PRIVATE ${VULKAN_INCLUDE_DIR})
target_link_libraries(vil PRIVATE glfw spdlog::spdlog VulkanMemoryAllocator glm)
target_include_directories(vil INTERFACE "Include/")