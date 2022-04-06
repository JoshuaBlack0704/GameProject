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
        uint32_t                                  stageCount;
        std::vector<VkPipelineShaderStageCreateInfo>           Stages;
        VkPipelineVertexInputStateCreateInfo      VertexInputState;
        VkPipelineInputAssemblyStateCreateInfo    InputAssemblyState;
        VkPipelineTessellationStateCreateInfo     TessellationState;
        VkPipelineViewportStateCreateInfo         ViewportState;
        VkPipelineRasterizationStateCreateInfo    RasterizationState;
        VkPipelineMultisampleStateCreateInfo      MultisampleState;
        VkPipelineDepthStencilStateCreateInfo     DepthStencilState;
        VkPipelineColorBlendStateCreateInfo       ColorBlendState;
        VkPipelineDynamicStateCreateInfo          DynamicState;
        VkPipelineLayout                          layout;
        VkRenderPass                              renderPass;
        uint32_t                                  subpass;
        VkPipeline                                basePipelineHandle;
        int32_t                                   basePipelineIndex;
        VkGraphicsPipelineCreateInfo gcInfo;
        VkComputePipelineCreateInfo ccInfo;
        VkPipelineLayoutCreateInfo lcInfo;
        vks::Image* image = nullptr;
        PipelineType type;
        std::vector<VkPushConstantRange> pushRanges;
    };


    class Pipeline
    {
    public:
        Pipeline(vksVkData &vkData, PipelineState state);
        std::function<void()> create;
        PipelineState state;
    private:
        VkDevice device;
        VkPipeline pipeline;
    };
}
