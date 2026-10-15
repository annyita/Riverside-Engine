#ifndef RIVERSIDE_APP_CLASS_H
#define RIVERSIDE_APP_CLASS_H

#include <window/window.h>
#include <render/vkCore/rsePipeline.h>
#include <render/vkCore/rseDevice.hpp>
#include <render/vkCore/rseSwapChain.hpp>

#include <memory>
#include <vector>

namespace RSE {

	class app {
	public:

		static constexpr int width = 800;
		static constexpr int height = 600;

		app();
		~app();

		app(const app&) = delete;
		app& operator=(const app&) = delete;

		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		Window mainWindow{width, height, "RiversideEngine"};
		rseDevice mainDevice{ mainWindow };
		rseSwapChain mainSwapChain{ mainDevice, mainWindow.getExtent() };
		//rsePipeline mainPipeline{mainDevice, "Asset/Shader/vertex.spv", "Asset/Shader/fragment.spv", rsePipeline::defaultPipelineConfigInfo(width, height) };
		std::unique_ptr<rsePipeline> mainPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}

#endif