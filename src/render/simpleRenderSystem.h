#pragma once

#include <render/vkCore/rsePipeline.h>
#include <render/vkCore/rseDevice.hpp>

#include <memory>
#include <vector>

namespace RSE {

	class simpleRenderSystem {
	public:

		simpleRenderSystem(rseDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~simpleRenderSystem();

		void recreateRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

		simpleRenderSystem(const simpleRenderSystem&) = delete;
		simpleRenderSystem& operator=(const simpleRenderSystem&) = delete;

		void renderEntities(VkCommandBuffer commandBuffer, VkDescriptorSet globalDescriptorSet);

	private:

		void renderMesh(VkCommandBuffer commandBuffer, auto& entityHandler);
		void renderModel(VkCommandBuffer commandBuffer, auto& entityHandler);

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		rseDevice& mainDevice;
		std::unique_ptr<rsePipeline> mainPipeline;
		VkPipelineLayout pipelineLayout;
	};
}