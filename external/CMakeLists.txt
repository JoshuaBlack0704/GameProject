add_subdirectory(glm)
add_subdirectory(VulkanMemoryAllocator)
add_subdirectory(glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include" CACHE INTERNAL "")
add_subdirectory(spdlog)
set(VMA_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator/include" CACHE INTERNAL "")


add_library(flecs STATIC "flecs/flecs.c" "flecs/flecs.h")
target_include_directories(flecs INTERFACE "flecs/")

file(GLOB imsrc "imgui/*.cpp")
file(GLOB imincl "imgui/*.h")
add_library(imgui STATIC ${imsrc} ${imincl} "imgui/backends/imgui_impl_glfw.h" "imgui/backends/imgui_impl_glfw.cpp" "imgui/backends/imgui_impl_vulkan.h" "imgui/backends/imgui_impl_vulkan.cpp")
target_include_directories(imgui PUBLIC "imgui/" ${GLFW_INCLUDE})
target_link_libraries(imgui PUBLIC Vulkan::Vulkan)