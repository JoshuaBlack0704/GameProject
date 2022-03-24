#pragma once
#define VULKAN_INTERMIDIATE_LIBRARY_NAMESPACE vil

struct MainData;

class VulkanInitalizer{
    public:
    void Init(bool withImGui = true);
    private:
    MainData* data;
};