file(GLOB src "Source/*.c" "Source/*.cpp")
file(GLOB incl "Include/*.h" "${GLFW_INCLUDE}/*.h" "${VMA_INCLUDE}/*.h" "$VKSINCLUDE/*.h")
set(COMMONINCLUDE "Include/" CACHE INTERNAL "")

add_library(common STATIC ${src} ${incl})

target_link_libraries(common PUBLIC spdlog::spdlog glm glfw flecs Vulkan::Vulkan imgui)
target_include_directories(common PUBLIC ${GLFW_INCLUDE} ${VMA_INCLUDE} ${VKSINCLUDE} "Include/")

add_subdirectory(Demos)