#pragma once
#include <vulkan/vulkan.h>
#include <vksPipeline.h>
#include <string>

void simpleTrianglePipeline(vks::PipelineState& state, std::vector<vks::ShaderInfo> shaderInfos, vkb::Swapchain swapchain){

    state.stages.clear();
    for(auto& info : shaderInfos){
        state.stages.emplace_back(info.stageInfo);
    }

    state.inputAssemblyState.primitiveRestartEnable = VK_FALSE;
    state.inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    state.viewPort.x = 0; state.viewPort.y = 0; state.viewPort.minDepth = 0; state.viewPort.maxDepth = 1;
    state.viewPort.width = swapchain.extent.width;
    state.viewPort.height = swapchain.extent.height;
    state.scissor.offset = {0,0};
    state.scissor.extent = swapchain.extent;
    state.viewportState.viewportCount = 1; state.viewportState.scissorCount = 1;
    state.viewportState.pViewports = &state.viewPort; state.viewportState.pScissors = & state.scissor;

    state.rasterizationState.depthClampEnable = VK_FALSE;
    state.rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    state.rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    state.rasterizationState.lineWidth = 1.0f;
    state.rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    state.rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    state.rasterizationState.depthBiasEnable = VK_FALSE;

    state.multisampleState.sampleShadingEnable = VK_FALSE;
    state.multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    state.multisampleState.minSampleShading = 1.0f; // Optional
    state.multisampleState.pSampleMask = nullptr; // Optional
    state.multisampleState.alphaToCoverageEnable = VK_FALSE; // Optional
    state.multisampleState.alphaToOneEnable = VK_FALSE; // Optional

    state.depthStencilState.
};
