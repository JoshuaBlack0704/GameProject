#include <vksDescriptors.h>
#include <vksTypes.h>
#include <spdlog/spdlog.h>

namespace vks{
    DescriptorPool::DescriptorPool(VkDevice device) : device(device) {
        VkDescriptorPoolCreateInfo cInfo = {};
        cInfo.sType = sType(cInfo);
        cInfo.maxSets = 1000;
        std::vector<VkDescriptorPoolSize> sizes = {
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, 1000},
                VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, 1000}
        };
        cInfo.poolSizeCount = sizes.size();
        cInfo.pPoolSizes = sizes.data();

        vkCreateDescriptorPool(device, &cInfo, nullptr, &descriptorPool);

        spdlog::info("Created Descriptor Pool");
    }

    void DescriptorPool::Dispose() {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        spdlog::info("Destroyed Descriptor Pool");
    }

    void DescriptorPool::AllocateSet(VkDescriptorSetAllocateInfo aInfo, std::vector<VkDescriptorSet> &sets) {
        aInfo.sType = sType(aInfo);
        aInfo.descriptorPool = descriptorPool;
        vkAllocateDescriptorSets(device, &aInfo, sets.data());
    }

    DescriptorSetLayout::DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout) : device(device), layout(layout) {}

    void DescriptorSetLayout::Dispose() {
        vkDestroyDescriptorSetLayout(device, layout, nullptr);
    }

    DescriptorBinding::DescriptorBinding(VkDescriptorType type, uint32_t count, VkShaderStageFlags stages, bool buffer, bool image, bool texelView,
                                         VkDescriptorBufferInfo bInfo, VkDescriptorImageInfo iInfo, VkBufferView tInfo)
            : image(image), buffer(buffer), texelView(texelView),
                                                                                             iInfo(iInfo), bInfo(bInfo), tInfo(tInfo){
        assert(((buffer) && (!image) && (!texelView)) || ((!buffer) && (image) && (!texelView)) || ((!buffer) && (!image) && (texelView)));
        binding = {};
        binding.descriptorType = type;
        binding.descriptorCount = count;
        binding.stageFlags = stages;
    }

    DescriptorSet::DescriptorSet(VkDevice device, std::shared_ptr<DescriptorSetLayout> copyLayout) : device(device), layout(copyLayout) {}

    void DescriptorSet::AddBinding(DescriptorBinding binding) {
        assert(layout == nullptr);
        binding.binding.binding = bindings.size();
        bindings.emplace_back(binding);
    }

    VkDescriptorSetLayout DescriptorSet::GetLayout() {
        if (layout != nullptr){
            return layout->layout;
        }

        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
        layoutBindings.reserve(bindings.size());
        for(auto& binding : bindings){
            layoutBindings.emplace_back(binding.binding);
        }

        VkDescriptorSetLayoutCreateInfo cInfo = {};
        cInfo.sType = sType(cInfo);
        cInfo.bindingCount = bindings.size();
        cInfo.pBindings = layoutBindings.data();

        VkDescriptorSetLayout newLayout;
        vkCreateDescriptorSetLayout(device, &cInfo, nullptr, &newLayout);

        layout = std::make_shared<DescriptorSetLayout>(device, newLayout);

        return layout->layout;
    }

    void DescriptorSet::Allocate(DescriptorPool pool) {
        std::vector<VkDescriptorSet> sets;
        sets.resize(1);

        VkDescriptorSetAllocateInfo aInfo = {};
        aInfo.sType = sType(aInfo);
        aInfo.descriptorSetCount = 1;
        auto layout  = GetLayout();
        aInfo.pSetLayouts = &layout;

        pool.AllocateSet(aInfo, sets);

        set = sets[0];
    }

    void DescriptorSet::Write() {
        assert(set != nullptr);
        std::vector<VkWriteDescriptorSet> writes = {};
        writes.reserve(bindings.size());
        for(auto& binding : bindings){
            VkWriteDescriptorSet write = {};
            write.sType = sType(write);
            write.dstSet = set;
            write.dstBinding = binding.binding.binding;
            write.dstArrayElement = 0;
            write.descriptorCount = binding.binding.descriptorCount;
            write.descriptorType = binding.binding.descriptorType;
            if (binding.buffer){
                write.pBufferInfo = &binding.bInfo;
            }
            else if(binding.image){
                write.pImageInfo = &binding.iInfo;
            }
            else if(binding.texelView){
                write.pTexelBufferView = &binding.tInfo;
            }
            writes.emplace_back(write);
        }
        vkUpdateDescriptorSets(device, writes.size(), writes.data(), 0, nullptr);
    }
}

