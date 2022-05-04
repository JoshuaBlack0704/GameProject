#include <PipelineStateSetters.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <vksTypes.h>
#include <BasicShapes.h>

void simpleTrianglePipeline(vks::PipelineState& state, std::vector<vks::ShaderInfo> shaderInfos, vkb::Swapchain swapchain){

    state.stages.clear();
    for(auto& info : shaderInfos){
        state.stages.emplace_back(info.stageInfo);
    }

    state.vBindings = VertexData::vertexInputBinding();
    state.vAttributes = VertexData::vertexInputAttribute();

    state.vertexInputState = {};
    state.vertexInputState.sType = vks::sType(state.vertexInputState);
    state.vertexInputState.vertexBindingDescriptionCount = state.vBindings.size();
    state.vertexInputState.vertexAttributeDescriptionCount = state.vAttributes.size();
    state.vertexInputState.pVertexBindingDescriptions = state.vBindings.data();
    state.vertexInputState.pVertexAttributeDescriptions = state.vAttributes.data();

    state.inputAssemblyState = {};
    state.inputAssemblyState.sType = vks::sType(state.inputAssemblyState);
    state.inputAssemblyState.primitiveRestartEnable = VK_FALSE;
    state.inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    state.tessellationState = {};
    state.tessellationState.sType = vks::sType(state.tessellationState);

    state.viewPort.x = 0; state.viewPort.y = 0; state.viewPort.minDepth = 0; state.viewPort.maxDepth = 1;
    state.viewPort.width = swapchain.extent.width;
    state.viewPort.height = swapchain.extent.height;
    state.scissor.offset = {0,0};
    state.scissor.extent = swapchain.extent;
    state.viewportState = {};
    state.viewportState.sType = vks::sType(state.viewportState);
    state.viewportState.viewportCount = 1; state.viewportState.scissorCount = 1;
    state.viewportState.pViewports = &state.viewPort; state.viewportState.pScissors = & state.scissor;

    state.rasterizationState = {};
    state.rasterizationState.sType = vks::sType(state.rasterizationState);
    state.rasterizationState.depthClampEnable = VK_FALSE;
    state.rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    state.rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    state.rasterizationState.lineWidth = 1.0f;
    state.rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    state.rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    state.rasterizationState.depthBiasEnable = VK_FALSE;

    state.multisampleState = {};
    state.multisampleState.sType = vks::sType(state.multisampleState);
    state.multisampleState.sampleShadingEnable = VK_FALSE;
    state.multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    state.multisampleState.minSampleShading = 1.0f; // Optional
    state.multisampleState.pSampleMask = nullptr; // Optional
    state.multisampleState.alphaToCoverageEnable = VK_FALSE; // Optional
    state.multisampleState.alphaToOneEnable = VK_FALSE; // Optional

    state.depthStencilState = {};
    state.depthStencilState.sType = vks::sType(state.depthStencilState);
    state.depthStencilState.depthTestEnable = VK_TRUE;
    state.depthStencilState.depthWriteEnable = VK_TRUE;
    state.depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.colorWriteMask = VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;
    state.colorBlendAttachments.emplace_back(colorBlendAttachment);

    state.colorBlendState = {};
    state.colorBlendState.sType = vks::sType(state.colorBlendState);
    state.colorBlendState.logicOpEnable = VK_FALSE;
    state.colorBlendState.attachmentCount = state.colorBlendAttachments.size();
    state.colorBlendState.pAttachments = state.colorBlendAttachments.data();

    state.dynamicRenderingInfo = {};
    state.dynamicRenderingInfo.sType = vks::sType(state.dynamicRenderingInfo);
    state.dynamicRenderingInfo.viewMask = 0;
    state.dynamicRenderingInfo.colorAttachmentCount = 1;
    state.dynamicRenderingInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
    state.dynamicRenderColorFormats.clear();
    state.dynamicRenderColorFormats.emplace_back(swapchain.image_format);
    state.dynamicRenderingInfo.pColorAttachmentFormats = state.dynamicRenderColorFormats.data();

};