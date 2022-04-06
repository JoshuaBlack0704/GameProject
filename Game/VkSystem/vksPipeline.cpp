#include <vksPipeline.h>
#include <spdlog/spdlog.h>
#include <tclDecls.h>

vks::Pipeline::Pipeline(vksVkData &vkData, PipelineState instate)
:device(vkData.lDevice.device), state(instate) {
    create = [&, instate](){
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
            }
            gcInfo.flags = 0;
            gcInfo.stageCount = state.stageCount;
            gcInfo.pStages = state.stageCount;
            gcInfo.pVertexInputState = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;
            gcInfo.stageCount = state.stageCount;

        }
        else{

        }

    };
}

