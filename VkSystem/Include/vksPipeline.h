#pragma once
#include <vksVkData.h>
#include <vksImage.h>
#include <functional>
#include <list>
#include <string>

namespace vks
{


    enum class PipelineType
    {
        GRAPHICS = 0, COMPUTE = 1
    };

    struct PipelineState{
        VkPipelineCreateFlags pipelineCreateFlags = {};
        std::vector<VkPipelineShaderStageCreateInfo>           stages = {};
        std::vector<VkVertexInputBindingDescription> vBindings;
        std::vector<VkVertexInputAttributeDescription> vAttributes;
        VkPipelineVertexInputStateCreateInfo      vertexInputState = {};
        VkPipelineInputAssemblyStateCreateInfo    inputAssemblyState = {};
        VkPipelineTessellationStateCreateInfo     tessellationState = {};
        VkViewport viewPort;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo         viewportState = {};
        VkPipelineRasterizationStateCreateInfo    rasterizationState = {};
        VkPipelineMultisampleStateCreateInfo      multisampleState = {};
        VkPipelineDepthStencilStateCreateInfo     depthStencilState = {};
        std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments = {};
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
        std::vector<VkFormat> dynamicRenderColorFormats = {};
        VkPipelineRenderingCreateInfo dynamicRenderingInfo = {};

        //Sets all of the sTypes for the state structs
        explicit PipelineState(PipelineType type = PipelineType::GRAPHICS);
    };


    class Pipeline
    {
    public:
        Pipeline(VkData &vkData);
        std::function<void()> create;
        PipelineState state;
        VkDevice device;
        void Dispose();
        ~Pipeline(){Dispose();}
    private:
        VkPipeline pipeline = nullptr;
    };
}
