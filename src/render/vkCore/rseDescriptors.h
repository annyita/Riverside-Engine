#pragma once

#include <render/vkCore/rseDevice.hpp>
// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace RSE {

    class rseDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(rseDevice& rseDevice) : mainDevice{ rseDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<rseDescriptorSetLayout> build() const;

        private:
            rseDevice& mainDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        rseDescriptorSetLayout(
            rseDevice& rseDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~rseDescriptorSetLayout();
        rseDescriptorSetLayout(const rseDescriptorSetLayout&) = delete;
        rseDescriptorSetLayout& operator=(const rseDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        rseDevice& mainDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class rseDescriptorWriter;
    };

    class rseDescriptorPool {
    public:
        class Builder {
        public:
            Builder(rseDevice& rseDevice) : mainDevice{ rseDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<rseDescriptorPool> build() const;

        private:
            rseDevice& mainDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        rseDescriptorPool(
            rseDevice& rseDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~rseDescriptorPool();
        rseDescriptorPool(const rseDescriptorPool&) = delete;
        rseDescriptorPool& operator=(const rseDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        rseDevice& mainDevice;
        VkDescriptorPool descriptorPool;

        friend class rseDescriptorWriter;
    };

    class rseDescriptorWriter {
    public:
        rseDescriptorWriter(rseDescriptorSetLayout& setLayout, rseDescriptorPool& pool);

        rseDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        rseDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        rseDescriptorSetLayout& setLayout;
        rseDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace lve