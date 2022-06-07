#include <vksPipeline.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vksTypes.h>



vks::Pipeline::Pipeline(VkData &vkData)
:device(vkData.lDevice.device), state(vks::PipelineType::GRAPHICS) {
    create = [&](){
        if (state.layout != nullptr){
            vkDestroyPipelineLayout(device, state.layout, nullptr);
        }
        if (pipeline != nullptr){
            vkDestroyPipeline(device, pipeline, nullptr);
        }

        if (state.type == PipelineType::GRAPHICS){
            state.gcInfo = {};
            VkGraphicsPipelineCreateInfo& gcInfo = state.gcInfo;
            gcInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            if (state.renderPass == nullptr){
                //We are using dynamic rendering with this gPipeline
                gcInfo.pNext = &state.dynamicRenderingInfo;
            }
            vkCreatePipelineLayout(device, &state.lcInfo, nullptr, &state.layout);

            gcInfo.flags = state.pipelineCreateFlags;
            gcInfo.stageCount = static_cast<uint32_t >(state.stages.size());
            gcInfo.pStages = state.stages.data();
            gcInfo.pVertexInputState = &state.vertexInputState;
            gcInfo.pInputAssemblyState = &state.inputAssemblyState;
            gcInfo.pTessellationState = &state.tessellationState;
            gcInfo.pViewportState = &state.viewportState;
            gcInfo.pRasterizationState = &state.rasterizationState;
            gcInfo.pMultisampleState = &state.multisampleState;
            gcInfo.pDepthStencilState = &state.depthStencilState;
            gcInfo.pColorBlendState = &state.colorBlendState;
            gcInfo.pDepthStencilState = &state.depthStencilState;
            gcInfo.layout = state.layout;
            gcInfo.renderPass = state.renderPass;
            gcInfo.subpass = state.subpass;
            gcInfo.basePipelineHandle = state.basePipelineHandle;
            gcInfo.basePipelineIndex = state.basePipelineIndex;

            vkCreateGraphicsPipelines(device, nullptr, 1, &gcInfo, nullptr, &pipeline);

            spdlog::info("Graphics Pipeline Created");
        }
        else{
            //We are making a compute pipeline

            VkComputePipelineCreateInfo& ccInfo = state.ccInfo;
            ccInfo.flags = state.pipelineCreateFlags;
            assert(state.stages.size() == 1);
            ccInfo.stage = state.stages[0];

            vkCreatePipelineLayout(device, &state.lcInfo, nullptr, &state.layout);

            ccInfo.layout = state.layout;
            ccInfo.basePipelineHandle = state.basePipelineHandle;
            ccInfo.basePipelineIndex = state.basePipelineIndex;

            vkCreateComputePipelines(device, nullptr, 1, &ccInfo, nullptr, &pipeline);
            spdlog::info("Compute Pipeline Created");

        }

    };
}

void vks::Pipeline::Dispose() {
    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, state.layout, nullptr);
}

vks::PipelineState::PipelineState(PipelineType type) : type(type){
    vertexInputState.sType = vks::sType(vertexInputState);
    inputAssemblyState.sType = vks::sType(inputAssemblyState);
    tessellationState.sType = vks::sType(tessellationState);
    viewportState.sType = vks::sType(viewportState);
    rasterizationState.sType = vks::sType(rasterizationState);
    multisampleState.sType = vks::sType(multisampleState);
    depthStencilState.sType = vks::sType(depthStencilState);
    colorBlendState.sType = vks::sType(colorBlendState);
    dynamicState.sType = vks::sType(dynamicState);
    gcInfo.sType = vks::sType(gcInfo);
    ccInfo.sType = vks::sType(ccInfo);
    lcInfo.sType = vks::sType(lcInfo);
    dynamicRenderingInfo.sType = vks::sType(dynamicRenderingInfo);
}

