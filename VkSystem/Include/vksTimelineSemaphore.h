#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace vks{

    class Semaphore{
    public:
        Semaphore(VkDevice device);
        void Dispose();
        ~Semaphore(){Dispose();}
        VkSemaphore semaphore;

    private:
        VkDevice device = nullptr;
    };

    class TimelineSemaphore{
    public:
        TimelineSemaphore(VkDevice device, uint64_t startingCount);
        void Wait(uint64_t timeout = UINT64_MAX);
        void SetSignalCount(uint64_t newCount);
        VkTimelineSemaphoreSubmitInfo GetSubmitInfo(bool incrementSignalCount, std::vector<TimelineSemaphore *> waitTimelineSemaphores, std::vector<Semaphore> waitNormalSemaphores);
        uint64_t GetSignalCount();
        void Signal();
        void IncrementSignalCount();
        VkSemaphore & GetSemaphore();
        void Dispose();
        ~TimelineSemaphore(){Dispose();}

    private:
        VkDevice device = nullptr;
        VkSemaphore timelineSemaphore = nullptr;
        uint64_t signalCount = 0;
        std::vector<uint64_t> waitValues = {};
        std::vector<VkSemaphore> waitSemaphores = {};
    };


}
