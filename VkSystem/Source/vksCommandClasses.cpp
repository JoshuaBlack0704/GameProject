#include <vksCmdClasses.h>
#include <spdlog/spdlog.h>

vks::CmdPoolWrapper::CmdPoolWrapper(VkDevice device, VkCommandPoolCreateInfo &cInfo) : device(device), cInfo(cInfo) {
    cInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    vkCreateCommandPool(device, &cInfo, nullptr, &cmdPool);
}

vks::CmdBufferSet& vks::CmdPoolWrapper::MakeSet() {
    CmdBufferSet set(device, cmdPool);
    cmdSets.emplace_back(set);
    return cmdSets.back();
}

void vks::CmdPoolWrapper::Reset() {
    vkResetCommandPool(device, cmdPool, 0);

    for (auto& set : cmdSets){
        set.Record();
    }

}

void vks::CmdPoolWrapper::Dispose() {
    vkDestroyCommandPool(device, cmdPool, nullptr);
    spdlog::info("Disposing CmdPoolWrapper");
}

vks::CmdBufferSet::CmdBufferSet(VkDevice device, VkCommandPool parentPool) : device(device), parentPool(parentPool){}

vks::CmdBufferSet& vks::CmdBufferSet::AddCmdBuffers(uint32_t count, VkCommandBufferLevel level) {
    VkCommandBufferAllocateInfo aInfo = {};
    aInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    aInfo.level = level;
    aInfo.commandPool = parentPool;
    aInfo.commandBufferCount = count;
    std::vector<VkCommandBuffer> newBuffers;
    newBuffers.resize(count);
    vkAllocateCommandBuffers(device, &aInfo, newBuffers.data());
    cmdBuffers.insert(cmdBuffers.begin(), newBuffers.begin(), newBuffers.end());
    return *this;
}

vks::CmdBufferSet& vks::CmdBufferSet::SetFunctions(std::vector<std::function<void(VkCommandBuffer)>> functions) {
    if (functions.size() != cmdBuffers.size()){
        spdlog::error("Command Buffer Set SetFunctions:\n   Functions size does not equal command buffer count.\n   Not all command buffers will be recorded");
    }
    operations.resize(functions.size());
    operations.insert(operations.begin(), functions.begin(), functions.end());

    return *this;
}

vks::CmdBufferSet &vks::CmdBufferSet::SetFunction(uint32_t index, std::function<void(VkCommandBuffer)> function) {
    operations[index] = function;
    return *this;
}

void vks::CmdBufferSet::Record() {
    for (int i = 0; i < cmdBuffers.size(); ++i) {
        //Remember, operations vector contains functions
        operations[i](cmdBuffers[i]);
    }
}
