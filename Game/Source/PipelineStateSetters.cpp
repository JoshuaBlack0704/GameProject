#include <PipelineStateSetters.h>

void gPipelineDefaultNoDepth(vks::PipelineState& state, std::vector<std::string> shaderPaths, std::vector<VkShaderStageFlags> shaderStages){
    state.pipelineCreateFlags = 0;
    state.stageCount = shaderPaths.size();
    for (int i = 0; i < shaderPaths.size(); ++i) {

    }
}