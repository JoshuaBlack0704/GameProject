#pragma once
#include <vksVkData.h>
#include <vksImage.h>
#include <functional>

namespace vks
{
    enum class PipelineType
    {
        GRAPHICS = 0, COMPUTE = 1
    };

    struct PipelineState{
        VkPipelineCreateFlags pipelineCreateFlags = {};
        uint32_t                                  stageCount = {};
        std::vector<VkShaderModule> shaders;
        std::vector<VkPipelineShaderStageCreateInfo>           stages = {};
        VkPipelineVertexInputStateCreateInfo      vertexInputState = {};
        VkPipelineInputAssemblyStateCreateInfo    inputAssemblyState = {};
        VkPipelineTessellationStateCreateInfo     tessellationState = {};
        VkPipelineViewportStateCreateInfo         viewportState = {};
        VkPipelineRasterizationStateCreateInfo    rasterizationState = {};
        VkPipelineMultisampleStateCreateInfo      multisampleState = {};
        VkPipelineDepthStencilStateCreateInfo     depthStencilState = {};
        VkPipelineColorBlendStateCreateInfo       colorBlendState = {};
        VkPipelineDynamicStateCreateInfo          dynamicState = {};
        VkPipelineLayout                          layout = {};
        VkRenderPass                              renderPass = nullptr;
        uint32_t                                  subpass = {};
        VkPipeline                                basePipelineHandle = {};
        int32_t                                   basePipelineIndex = {};
        VkGraphicsPipelineCreateInfo gcInfo  = {};
        VkComputePipelineCreateInfo ccInfo  = {};
        VkPipelineLayoutCreateInfo lcInfo  = {};
        vks::Image* image = nullptr;
        PipelineType type;
        std::vector<VkPushConstantRange> pushRanges = {};
        VkPipelineRenderingCreateInfo dynamicRenderingInfo = {};

        //Sets all of the sTypes for the state structs
        explicit PipelineState(PipelineType type = PipelineType::GRAPHICS);
    };


    class Pipeline
    {
    public:
        Pipeline(vksVkData &vkData, PipelineState& state);
        std::function<void()> create;
        PipelineState state;
        void Dispose();
        ~Pipeline(){Dispose();}
    private:
        VkDevice device;
        VkPipeline pipeline;
    };
}
