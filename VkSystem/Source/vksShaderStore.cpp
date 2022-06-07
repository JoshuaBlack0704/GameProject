#include <vector>
#include <iostream>
#include <vksShaderStore.h>
#include <vksTypes.h>
#include <fstream>

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