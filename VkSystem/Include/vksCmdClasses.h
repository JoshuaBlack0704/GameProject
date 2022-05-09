#pragma once
#include <vulkan/vulkan.h>
#include <functional>
#include <list>
#include <vector>
#include <string>

namespace vks
{
    class CmdBufferSet{
    public:
        CmdBufferSet(VkDevice device, VkCommandPool parentPool, std::string name);
        vks::CmdBufferSet &AddCmdBuffers(uint32_t count, VkCommandBufferLevel level);
        ///Sets the functions executed by the classes command buffers
        ///This requires that the functions vector contain the same number of functions as there are command buffers
        CmdBufferSet& SetFunctions(std::vector<std::function<void(VkCommandBuffer)>> functions);
        ///Sets the function for a command buffer indicated be the index argument
        CmdBufferSet& SetFunction(uint32_t  index, std::function<void(VkCommandBuffer)> function);
        VkCommandBuffer* pCommandBuffers();
        void Record();

    private:
        VkDevice device;
        VkCommandPool parentPool;
        std::vector<VkCommandBuffer> cmdBuffers;
        std::vector<std::function<void(VkCommandBuffer)>> operations;
        std::string name;
    };

    class CmdPoolWrapper{
    public:
        CmdPoolWrapper(VkDevice device, VkCommandPoolCreateInfo &pcInfo, std::string name);
        VkDevice device;
        VkCommandPool cmdPool;
        void Dispose();
        CmdBufferSet& MakeSet(std::string cmdSetName);
        //When we reset the command pool, all of the the children cmdSets will be re-recorded
        void Reset();
        ~CmdPoolWrapper(){Dispose();};
    private:
        std::list<CmdBufferSet> cmdSets;
        VkCommandPoolCreateInfo cInfo;
        std::string name;
    };


}

