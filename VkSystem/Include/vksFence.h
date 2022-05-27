#pragma once
#include <vulkan/vulkan.h>

namespace vks{
    class Fence{
    public:
        Fence(VkDevice device, VkFenceCreateFlags flags);
        void Wait();
        void Reset();
        void Dispose();
        VkFence GetFence(){return fence;}
        ~Fence(){Dispose();}


    private:
        VkDevice device;
        VkFence fence;
    };
}