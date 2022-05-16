cmake_minimum_required(VERSION 3.0.0)
project(Game VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Vulkan REQUIRED)

add_subdirectory(external)
add_subdirectory(VkSystem)
add_subdirectory(CommonSystems)
#add_subdirectory(Game)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
