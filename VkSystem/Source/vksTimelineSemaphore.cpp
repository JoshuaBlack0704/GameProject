#include <vksTimelineSemaphore.h>
#include <vksTypes.h>
#include <cassert>
#include <vector>

namespace vks{

    TimelineSemaphore::TimelineSemaphore(VkDevice device, uint64_t startingCount) : device(device) {

        VkSemaphoreTypeCreateInfo tcInfo = {};
        tcInfo.sType = sType(tcInfo);
        tcInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
        tcInfo.initialValue = startingCount;
        signalCount = startingCount;

        VkSemaphoreCreateInfo scInfo;
        scInfo.sType = sType(scInfo);
        scInfo.pNext = &tcInfo;
        vkCreateSemaphore(device, &scInfo, nullptr, &timelineSemaphore);
    }

    void TimelineSemaphore::Dispose() {
        vkDestroySemaphore(device, timelineSemaphore, nullptr);
    }

    void TimelineSemaphore::SetSignalCount(uint64_t newCount) {
        assert(newCount > signalCount);
        signalCount = newCount;
    }

    uint64_t TimelineSemaphore::GetSignalCount() {return signalCount;}

    void TimelineSemaphore::IncrementSignalCount() {signalCount++;}

    VkTimelineSemaphoreSubmitInfo TimelineSemaphore::GetSubmitInfo(bool incrementSignalCount, std::vector<TimelineSemaphore *> waitTimelineSemaphores) {
        if (incrementSignalCount) {IncrementSignalCount();}

        waitValues.clear();
        waitSemaphores.clear();

        for (auto ts : waitTimelineSemaphores){
            waitValues.emplace_back(ts->GetSignalCount());
            waitSemaphores.emplace_back(ts->GetSemaphore());
        }

        VkTimelineSemaphoreSubmitInfo tsInfo = {};
        tsInfo.sType = sType(tsInfo);
        tsInfo.waitSemaphoreValueCount = waitValues.size();
        tsInfo.pWaitSemaphoreValues = waitValues.data();
        tsInfo.signalSemaphoreValueCount = 1;
        tsInfo.pSignalSemaphoreValues = &signalCount;

        return tsInfo;

    }

    VkSemaphore TimelineSemaphore::GetSemaphore() {return timelineSemaphore;}

    void TimelineSemaphore::Wait() {
        VkSemaphoreWaitInfo waitInfo = {};
        waitInfo.sType = sType(waitInfo);
    }
}
