#pragma once

#include <render/vkCore/rsePipeline.h>
#include <render/vkCore/rseDevice.hpp>

#include <memory>
#include <vector>

namespace RSE {

	class simpleRenderSystem {
	public:

		simpleRenderSystem(rseDevice& device, VkRenderPass renderPass);
		~simpleRenderSystem();

		simpleRenderSystem(const simpleRenderSystem&) = delete;
		simpleRenderSystem& operator=(const simpleRenderSystem&) = delete;

		void renderEntities(VkCommandBuffer commandBuffer);

	private:

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		rseDevice& mainDevice;
		std::unique_ptr<rsePipeline> mainPipeline;
		VkPipelineLayout pipelineLayout;
	};
}