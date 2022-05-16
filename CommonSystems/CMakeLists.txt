file(GLOB src "Source/*.c" "Source/*.cpp")
file(GLOB incl "Include/*.h" "${GLFW_INCLUDE}/*.h")

add_library(common STATIC ${src} ${incl})

target_link_libraries(common PUBLIC spdlog::spdlog glm glfw flecs VkSystem imgui)
target_include_directories(common PUBLIC ${GLFW_INCLUDE} "Include/")

add_subdirectory(Demos)