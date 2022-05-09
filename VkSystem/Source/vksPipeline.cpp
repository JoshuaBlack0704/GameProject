#include <vksPipeline.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vksTypes.h>

void PathToBytes(std::string path, std::vector<char>& outputBuffer){
    std::string data;
    std::ifstream readStream;
    readStream.open(path, std::ios::ate | std::ios::binary);
    uint64_t size = readStream.tellg();
    outputBuffer.resize(size);
    readStream.seekg(0);
    readStream.read(outputBuffer.data(), static_cast<uint32_t>(size));
    readStream.close();
}


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
            gcInfo.stageCount = state.stages.size();
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

vks::ShaderStore::ShaderStore(VkDevice device) :device(device){}

vks::ShaderInfo& vks::ShaderStore::AddShader(std::string filePath, VkShaderStageFlagBits stage, std::string entryPoint,
                                     const void *pNext, VkPipelineShaderStageCreateFlagBits flags,
                                     const VkSpecializationInfo * specInfo) {
    shaderInfos.emplace_back(vks::ShaderInfo{filePath,stage,entryPoint,pNext,flags,specInfo});
    return shaderInfos.back();
}

void vks::ShaderStore::ReloadShaders() {
    Dispose();
    for(auto& info : shaderInfos){
        std::vector<char> code(1);
        PathToBytes(info.filePath, code);

        VkShaderModuleCreateInfo moduleCreateInfo = {};
        moduleCreateInfo.sType = vks::sType(moduleCreateInfo);
        moduleCreateInfo.pNext = info.pNext;
        moduleCreateInfo.flags = info.flags;
        moduleCreateInfo.codeSize = code.size();
        moduleCreateInfo.pCode = reinterpret_cast<uint32_t *>(code.data());
        vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &info.module);

        info.stageInfo = {};
        info.stageInfo.sType = vks::sType(info.stageInfo);
        info.stageInfo.module = info.module;
        info.stageInfo.stage = info.stage;
        info.stageInfo.pName = info.entryPoint.c_str();
        info.stageInfo.pSpecializationInfo = info.specInfo;


    }
}

void vks::ShaderStore::Dispose() {
    for (auto& info : shaderInfos){
        vkDestroyShaderModule(device, info.module, nullptr);
    }
}