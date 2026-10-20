#include "rseRenderer.h"

#include <stdexcept>
#include <array>
#include <util/rseTime.h>

#include <util/rseMath.h>

namespace RSE {
	rseRenderer::rseRenderer(Window& window, rseDevice& device) : mainWindow{ window }, mainDevice{ device } {
		recreateSwapChain();
		createCommandBuffers();
	}

	rseRenderer::~rseRenderer(){
		freeCommandBuffes();
	}

	VkCommandBuffer rseRenderer::beginFrame(){
		assert(!isFrameStarted && "Cant call beginFrame while already in progress");

		auto result = mainSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		return commandBuffer;
	}

	void rseRenderer::endFrame(){
		assert(isFrameStarted && "Cant call endFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}

		auto result = mainSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mainWindow.wasWindowResized()) {
			mainWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % rseSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void rseRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
		assert(isFrameStarted && "Cant call beginSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "cant begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mainSwapChain->getRenderPass();
		renderPassInfo.framebuffer = mainSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = mainSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.3f, 0.3f, 0.9f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(mainSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(mainSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ { 0, 0 }, mainSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void rseRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
		assert(isFrameStarted && "Cant call endSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "cant end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void rseRenderer::createCommandBuffers(){
		commandBuffers.resize(rseSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = mainDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(mainDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
	}

	void rseRenderer::freeCommandBuffes(){
		vkFreeCommandBuffers(mainDevice.device(), mainDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}
	
	void rseRenderer::recreateSwapChain(){
		auto extent = mainWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = mainWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(mainDevice.device());

		if (mainSwapChain == nullptr) {
			mainSwapChain = nullptr; // added here to prevent crash
			mainSwapChain = std::make_unique<rseSwapChain>(mainDevice, extent);
		}
		else {
			std::shared_ptr<rseSwapChain> oldSwapChain = std::move(mainSwapChain);

			mainSwapChain = nullptr; // added here to prevent crash (idk if this one will create an issue)
			mainSwapChain = std::make_unique<rseSwapChain>(mainDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*mainSwapChain.get())) {
				throw std::runtime_error("Swap chain image(or depth) format has changed");
			}

			//if (mainSwapChain->imageCount() != commandBuffers.size()) {
			//	freeCommandBuffes();
			//	createCommandBuffers();
			//}
		}
	}
}