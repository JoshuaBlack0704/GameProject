#include <vksTypes.h>


namespace vks
{
    VkStructureType sType(VkShaderModuleCreateInfo&){return VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;}
    VkStructureType sType(VkPipelineShaderStageCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;}
    VkStructureType sType(VkPipelineVertexInputStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineInputAssemblyStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineTessellationStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineViewportStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineRasterizationStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineMultisampleStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineDepthStencilStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineColorBlendStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;}
    VkStructureType sType(VkPipelineDynamicStateCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;}
    VkStructureType sType(VkGraphicsPipelineCreateInfo&){return VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;}
    VkStructureType sType(VkComputePipelineCreateInfo&){return VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;}
    VkStructureType sType(VkPipelineLayoutCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;}
    VkStructureType sType(VkPipelineRenderingCreateInfo&){return VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;}
    VkStructureType sType(VkBufferCreateInfo&){return VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;}
    VkStructureType sType(VkImageCreateInfo&){return VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;}
    VkStructureType sType(VkSubmitInfo&){return VK_STRUCTURE_TYPE_SUBMIT_INFO;}
    VkStructureType sType(VkMemoryBarrier&){return VK_STRUCTURE_TYPE_MEMORY_BARRIER;}
    VkStructureType sType(VkCommandBufferBeginInfo&){return VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;}

}