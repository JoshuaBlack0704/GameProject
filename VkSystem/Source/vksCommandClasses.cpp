#include <vksCmdClasses.h>
#include <spdlog/spdlog.h>

vks::CmdPoolWrapper::CmdPoolWrapper(VkDevice device, VkCommandPoolCreateInfo &cInfo, std::string name) : device(device), cInfo(cInfo), name(name) {
    cInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    vkCreateCommandPool(device, &cInfo, nullptr, &cmdPool);
    spdlog::info("Creating CmdPoolWrapper class \"{}\"", name);
}

vks::CmdBufferSet& vks::CmdPoolWrapper::MakeSet(std::string cmdSetName) {
    CmdBufferSet set(device, cmdPool, cmdSetName);
    cmdSets.emplace_back(set);
    spdlog::info("CmdPoolWrapper Class \"{}\" created a CmdSetClass \"{}\"", name, cmdSetName);
    return cmdSets.back();
}

void vks::CmdPoolWrapper::Reset(bool Record) {
    vkResetCommandPool(device, cmdPool, 0);

    if  (Record){

        for (auto& set : cmdSets){
            set.Record();
        }
    }

}

void vks::CmdPoolWrapper::Dispose() {
    vkDestroyCommandPool(device, cmdPool, nullptr);
    spdlog::info("Disposing CmdPoolWrapper class \"{}\"", name);
}

vks::CmdBufferSet::CmdBufferSet(VkDevice device, VkCommandPool parentPool, std::string name) : device(device), parentPool(parentPool), name(name) {}

vks::CmdBufferSet& vks::CmdBufferSet::AddCmdBuffers(uint32_t count, VkCommandBufferLevel level) {
    VkCommandBufferAllocateInfo aInfo = {};
    aInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    aInfo.level = level;
    aInfo.commandPool = parentPool;
    aInfo.commandBufferCount = count;
    std::vector<VkCommandBuffer> newBuffers;
    newBuffers.resize(count);
    vkAllocateCommandBuffers(device, &aInfo, newBuffers.data());
    cmdBuffers.insert(cmdBuffers.end(), newBuffers.begin(), newBuffers.end());
    return *this;
}

vks::CmdBufferSet& vks::CmdBufferSet::SetFunctions(std::vector<std::function<void(VkCommandBuffer)>> functions) {
    operations.insert(operations.begin(), functions.begin(), functions.end());
    if (cmdBuffers.size() != operations.size()){
        uint32_t difference = static_cast<uint32_t >(operations.size() - cmdBuffers.size());
        AddCmdBuffers(difference, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }
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

VkCommandBuffer *vks::CmdBufferSet::pCommandBuffers() {
    return cmdBuffers.data();}