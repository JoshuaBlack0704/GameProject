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

        VkSemaphoreCreateInfo scInfo = {};
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

    VkTimelineSemaphoreSubmitInfo TimelineSemaphore::GetSubmitInfo(bool incrementSignalCount, std::vector<TimelineSemaphore *> waitTimelineSemaphores, std::vector<Semaphore> waitNormalSemaphores) {
        if (incrementSignalCount) {IncrementSignalCount();}

        waitValues.clear();
        waitSemaphores.clear();

        for (auto ts : waitTimelineSemaphores){
            waitValues.emplace_back(ts->GetSignalCount());
            waitSemaphores.emplace_back(ts->GetSemaphore());
        }
        for(auto s : waitNormalSemaphores){
            waitValues.emplace_back(0);
            waitSemaphores.emplace_back(s.semaphore);
        }

        VkTimelineSemaphoreSubmitInfo tsInfo = {};
        tsInfo.sType = sType(tsInfo);
        tsInfo.waitSemaphoreValueCount = static_cast<uint32_t>(waitValues.size());
        tsInfo.pWaitSemaphoreValues = waitValues.data();
        tsInfo.signalSemaphoreValueCount = 1;
        tsInfo.pSignalSemaphoreValues = &signalCount;

        return tsInfo;

    }

    VkSemaphore & TimelineSemaphore::GetSemaphore() {return timelineSemaphore;}

    void TimelineSemaphore::Wait(uint64_t timeout) {
        VkSemaphoreWaitInfo waitInfo = {};
        waitInfo.sType = sType(waitInfo);
        waitInfo.semaphoreCount = 1;
        waitInfo.pSemaphores = &timelineSemaphore;
        waitInfo.pValues = &signalCount;
        vkWaitSemaphores(device, &waitInfo, timeout);
    }

    void TimelineSemaphore::Signal() {
        VkSemaphoreSignalInfo signalInfo = {};
        signalInfo.sType = sType(signalInfo);
        signalInfo.semaphore = timelineSemaphore;
        signalInfo.value = signalCount;
        vkSignalSemaphore(device, &signalInfo);
    }

    Semaphore::Semaphore(VkDevice device) : device(device) {
        VkSemaphoreCreateInfo cInfo = {};
        cInfo.sType = sType(cInfo);
        vkCreateSemaphore(device, &cInfo, nullptr, &semaphore);
    }

    void Semaphore::Dispose() {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
}
