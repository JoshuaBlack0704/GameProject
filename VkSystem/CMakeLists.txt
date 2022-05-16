file(GLOB_RECURSE src "*.cpp")
file(GLOB_RECURSE incl "*.h")

add_library(VkSystem STATIC ${src} ${incl})
if(MSVC)
    target_compile_options(VkSystem PRIVATE /MP)
endif()
target_link_libraries(VkSystem PUBLIC spdlog::spdlog Vulkan::Vulkan VkBootstrap vma glfw)
target_include_directories(VkSystem PUBLIC "Include/")