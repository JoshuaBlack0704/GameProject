file(GLOB_RECURSE src "*.c" "*.cpp")
file(GLOB_RECURSE incl "*.hpp" "*.h" "${GLFW_INCLUDE}/*.h" "${VMA_INCLUDE}/*.h")

add_executable(game ${src} ${incl})

target_link_libraries(game PUBLIC spdlog::spdlog glm glfw flecs Vulkan::Vulkan imgui)
target_include_directories(game PUBLIC ${GLFW_INCLUDE} ${VMA_INCLUDE} "VkSystem/" "VkBootstrap/")
