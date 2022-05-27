#include <vksFence.h>
#include <vksTypes.h>

namespace vks{
    Fence::Fence(VkDevice device, VkFenceCreateFlags flags) : device(device) {
        VkFenceCreateInfo cInfo = {};
        cInfo.sType = vks::sType(cInfo);
        cInfo.flags = flags;
        vkCreateFence(device, &cInfo, nullptr, &fence);
    }

    void Fence::Dispose() {
        vkDestroyFence(device, fence, nullptr);
    }

    void Fence::Wait() {
        vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
    }

    void Fence::Reset() {
        vkResetFences(device, 1, &fence);
    }
}