file(GLOB_RECURSE src "*.c" "*.cpp")
file(GLOB_RECURSE VkSystemInclude "*.hpp" "*.h" "${VMA_INCLUDE}/*.h")
set(VKSINCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Include/" CACHE INTERNAL "")

add_library(VkSystem STATIC ${src} ${VkSystemInclude})

target_link_libraries(VkSystem PUBLIC spdlog::spdlog glm Vulkan::Vulkan imgui)
target_include_directories(VkSystem PUBLIC ${VMA_INCLUDE} "Include/")