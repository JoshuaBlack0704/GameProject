file(GLOB_RECURSE src "*.c" "*.cpp")
file(GLOB_RECURSE incl "*.hpp" "*.h")

add_executable(game ${src} ${incl})

target_link_libraries(game PRIVATE spdlog::spdlog glm vkLayer)
