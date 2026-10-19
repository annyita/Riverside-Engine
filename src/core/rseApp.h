#ifndef RIVERSIDE_APP_CLASS_H
#define RIVERSIDE_APP_CLASS_H

#include <window/window.h>
#include <render/vkCore/rsePipeline.h>
#include <render/vkCore/rseDevice.hpp>
#include <render/vkCore/rseSwapChain.hpp>

#include <render/object/rseMesh.h>
#include <game/scene/entityHandler.h>

#include <memory>
#include <vector>

namespace RSE {

	class app {
	public:

		static constexpr int width = 1920;
		static constexpr int height = 1080;

		app();
		~app();

		app(const app&) = delete;
		app& operator=(const app&) = delete;

		void run();

	private:


		void loadEntities();
		
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffes();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderEntities(VkCommandBuffer commandBuffer);

		Window mainWindow{width, height, "Riverside Engine"};
		rseDevice mainDevice{ mainWindow };
		//rseSwapChain mainSwapChain{ mainDevice, mainWindow.getExtent() };
		std::unique_ptr<rseSwapChain> mainSwapChain;
		//rsePipeline mainPipeline{mainDevice, "Asset/Shader/vertex.spv", "Asset/Shader/fragment.spv", rsePipeline::defaultPipelineConfigInfo(width, height) };
		std::unique_ptr<rsePipeline> mainPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}

#endif