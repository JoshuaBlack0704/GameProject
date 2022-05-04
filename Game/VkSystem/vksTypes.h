#pragma once

#include <vulkan/vulkan.h>

namespace vks
{
    VkStructureType sType(VkShaderModuleCreateInfo&);
    VkStructureType sType(VkPipelineShaderStageCreateInfo&);
    VkStructureType sType(VkPipelineVertexInputStateCreateInfo&);
    VkStructureType sType(VkPipelineInputAssemblyStateCreateInfo&);
    VkStructureType sType(VkPipelineTessellationStateCreateInfo&);
    VkStructureType sType(VkPipelineViewportStateCreateInfo&);
    VkStructureType sType(VkPipelineRasterizationStateCreateInfo&);
    VkStructureType sType(VkPipelineMultisampleStateCreateInfo&);
    VkStructureType sType(VkPipelineDepthStencilStateCreateInfo&);
    VkStructureType sType(VkPipelineColorBlendStateCreateInfo&);
    VkStructureType sType(VkPipelineDynamicStateCreateInfo&);
    VkStructureType sType(VkGraphicsPipelineCreateInfo&);
    VkStructureType sType(VkComputePipelineCreateInfo&);
    VkStructureType sType(VkPipelineLayoutCreateInfo&);
    VkStructureType sType(VkPipelineRenderingCreateInfo&);

}