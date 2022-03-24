file(GLOB src "Source/*.cpp")
file(GLOB incl "Include/*.hpp")

add_executable(game ${src} ${incl})

target_link_libraries(game PRIVATE spdlog::spdlog glm vil)
