#pragma once

#include <window/window.h>
#include <render/vkCore/rseDevice.hpp>
#include <render/vkCore/rseSwapChain.hpp>

#include <cassert>
#include <memory>
#include <vector>

namespace RSE {

	class rseRenderer {
	public:
		rseRenderer(Window &window, rseDevice &device);
		~rseRenderer();

		rseRenderer(const rseRenderer&) = delete;
		rseRenderer& operator=(const rseRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return mainSwapChain->getRenderPass(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:

		void createCommandBuffers();
		void freeCommandBuffes();
		void recreateSwapChain();

		Window &mainWindow;
		rseDevice& mainDevice;
		std::unique_ptr<rseSwapChain> mainSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted = false;
	};
}