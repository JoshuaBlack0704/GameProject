#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace vks{

    struct DescriptorBinding{
        DescriptorBinding(VkDescriptorType type, uint32_t count, VkShaderStageFlags stages, bool buffer, bool image, bool texelView,
                          VkDescriptorBufferInfo bInfo, VkDescriptorImageInfo iInfo, VkBufferView tInfo);
        bool image, buffer, texelView;
        VkDescriptorImageInfo iInfo = {};
        VkDescriptorBufferInfo bInfo = {};
        VkBufferView tInfo = {};
        VkDescriptorSetLayoutBinding binding = {};
    };

    struct DescriptorSetLayout{
        VkDevice device = nullptr;
        VkDescriptorSetLayout layout = nullptr;
        DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout);
        void Dispose();
        ~DescriptorSetLayout(){Dispose();}
    };

    class DescriptorPool{
    public:
        explicit DescriptorPool(VkDevice device);

        void AllocateSet(VkDescriptorSetAllocateInfo aInfo, VkDescriptorSet &set);


        void Dispose();
        ~DescriptorPool(){Dispose();}
    private:
        VkDevice device = nullptr;
        VkDescriptorPool descriptorPool = nullptr;

    };

    class DescriptorSet{
    public:
        explicit DescriptorSet(VkDevice device, std::shared_ptr<DescriptorSetLayout> copyLayout = nullptr);
        void AddBinding(DescriptorBinding binding);
        VkDescriptorSetLayout GetLayout();
        void Allocate(DescriptorPool &pool);
        void Write();

    private:
        VkDevice device = nullptr;
        std::shared_ptr<DescriptorSetLayout> layout = nullptr;
        VkDescriptorSet set = nullptr;
        std::vector<DescriptorBinding> bindings;
    };



}