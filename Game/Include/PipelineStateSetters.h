#pragma once
#include <vulkan/vulkan.h>
#include <vksPipeline.h>
#include <string>

void gPipelineDefaultNoDepth(vks::PipelineState& state, std::vector<std::string> shaderPaths, std::vector<VkShaderStageFlags> shaderStages);
