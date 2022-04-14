#include <PipelineStateSetters.h>
#include <vector>
#include <FileIO.h>

void gPipelineDefaultNoDepth(vks::Pipeline& pipeline, std::vector<std::string> shaderPaths, std::vector<VkShaderStageFlagBits> shaderStages){
    pipeline.state.pipelineCreateFlags = 0;
    pipeline.state.stageCount = shaderPaths.size();
    std::vector<char> shaderData;
    std::vector<VkShaderModule> modules(shaderData.size());
    for (int i = 0; i < pipeline.state.stageCount; ++i) {
        shaderData.clear();
        fio::PathToBytes(shaderPaths[i], shaderData);

        VkShaderModuleCreateInfo mcInfo = {};
        mcInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        mcInfo.codeSize = shaderData.size();
        mcInfo.pCode = reinterpret_cast<const uint32_t *>(shaderData.data());
        modules.emplace_back();
        vkCreateShaderModule(pipeline.device, &mcInfo, nullptr, &modules.back());

        VkPipelineShaderStageCreateInfo stageInfo = {};
        stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageInfo.stage = shaderStages[i];
        stageInfo.module = modules.back();
        stageInfo.pName = "main";


        pipeline.state.stages.emplace_back(stageInfo);
    }



}