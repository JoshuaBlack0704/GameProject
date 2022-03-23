cmake_minimum_required(VERSION 3.0.0)
project(Game VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Vulkan REQUIRED)
find_package(glfw3)
find_package(spdlog)

add_subdirectory(external)

add_executable(Game main.cpp)

target_include_directories(Game PRIVATE ${VULKAN_INCLUDE_DIR})
target_link_libraries(Game PRIVATE glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
