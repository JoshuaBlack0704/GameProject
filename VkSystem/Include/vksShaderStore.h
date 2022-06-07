#pragma once
#include <string>
#include <list>
#include <vulkan/vulkan.h>


namespace vks {


    struct ShaderInfo{
        std::string filePath = {};
        VkShaderStageFlagBits stage = {};
        std::string entryPoint = {};
        const void* pNext = nullptr;
        VkPipelineShaderStageCreateFlagBits flags = {};
        const VkSpecializationInfo* specInfo = {};
        VkShaderModule module = {};
        VkPipelineShaderStageCreateInfo stageInfo = {};
    };

    class ShaderStore{
    public:
        ShaderStore(VkDevice device);

        ShaderInfo& AddShader(std::string filePath, VkShaderStageFlagBits stage, std::string entryPoint, const void* pNext = nullptr, VkPipelineShaderStageCreateFlagBits flags = {}, const VkSpecializationInfo* = {});
        void ReloadShaders();
        void Dispose();
        ~ShaderStore(){Dispose();};

    private:
        VkDevice device;
        std::list<ShaderInfo> shaderInfos;
    };


}

