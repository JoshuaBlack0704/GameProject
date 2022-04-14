#include <vksPipeline.h>
#include <spdlog/spdlog.h>
#include <FileIO.h>

vks::Pipeline::Pipeline(vksVkData &vkData)
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
            gcInfo.stageCount = state.stageCount;
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
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    gcInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    ccInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    lcInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    dynamicRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
}

vks::ShaderStore::ShaderStore(VkDevice device) :device(device){}

uint64_t vks::ShaderStore::AddShader(std::string filePath, VkShaderStageFlagBits stage, std::string entryPoint,
                                     const void *pNext, VkPipelineShaderStageCreateFlagBits flags,
                                     const VkSpecializationInfo * specInfo) {
    std::vector<char> code(10);
    fio::PathToBytes(filePath, code);

    VkShaderModuleCreateInfo moduleCreateInfo = {};
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.codeSize = code.size();
    moduleCreateInfo.pCode = reinterpret_cast<uint32_t *>(code.data());

    VkShaderModule module;
    //vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &module);

    VkPipelineShaderStageCreateInfo stageCreateInfo = {};
    stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageCreateInfo.pNext = pNext;
    stageCreateInfo.flags = flags;
    stageCreateInfo.stage = stage;
    stageCreateInfo.module = module;
    entryPoints.emplace_back(entryPoint);
    stageCreateInfo.pName = entryPoints.back().c_str();
    stageCreateInfo.pSpecializationInfo = specInfo;

    shaderModules.emplace_back();
    shaderStageInfos.emplace_back(stageCreateInfo);

    return  shaderStageInfos.size()-1;

}

void vks::ShaderStore::DeleteShader(uint64_t shaderIndex) {
    shaderStageInfos.erase(shaderStageInfos.begin() + shaderIndex);
}