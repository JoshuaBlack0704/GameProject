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
    VkStructureType sType(VkBufferCreateInfo&);
    VkStructureType sType(VkImageCreateInfo&);
    VkStructureType sType(VkSubmitInfo&);
    VkStructureType sType(VkMemoryBarrier&);
    VkStructureType sType(VkCommandBufferBeginInfo&);
    VkStructureType sType(VkSemaphoreCreateInfo&);
    VkStructureType sType(VkSemaphoreTypeCreateInfo&);
    VkStructureType sType(VkTimelineSemaphoreSubmitInfo&);
}