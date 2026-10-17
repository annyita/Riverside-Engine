#include <rseApp.h>
#include <stdexcept>
#include <array>
#include <util/rseTime.h>

namespace RSE {

	app::app() {
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}
	app::~app(){
		vkDestroyPipelineLayout(mainDevice.device(), pipelineLayout, nullptr);
	}
	
	void app::run() {

		while (!mainWindow.shouldClose()) {
			glfwPollEvents();
			rseTime::updateTimer(rseTime::mainTimer);
			drawFrame();
		}
	}

	/*
				{{ 0.0f, -0.5f},{1.0, 0.0, 0.0} },
			{{0.5f, 0.5f},{ 0.0, 1.0, 0.0 } },
			{{-0.5f, 0.5f},{ 0.0, 0.0, 1.0 } },
	*/

	void app::loadModels(){
		std::vector<Model::vertex> vertices{
			{{-1.0f, -1.0f},{0.0, 0.0, 0.0} },
			{{1.0f, -1.0f},{ 1.0, 0.0, 0.0 } },
			{{1.0f, 1.0f},{ 1.0, 1.0, 0.0 } },
			{{1.0f, 1.0f },{ 1.0, 1.0, 0.0 } },
			{{-1.0f, 1.0f },{ 0.0, 1.0, 0.0 } },
			{{-1.0f, -1.0f },{ 0.0, 0.0, 0.0 } }
		};

		mainModel = std::make_unique<Model>(mainDevice, vertices);
	}

	void app::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(mainDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout");
		}

	}

	void app::createPipeline(){
		pipelineCfgInfo pipelineConfig{};
		rsePipeline::defaultPipelineConfigInfo(pipelineConfig); // mainSwapChain->getSwapChainExtent().width, mainSwapChain->getSwapChainExtent().height
		pipelineConfig.renderPass = mainSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		mainPipeline = std::make_unique<rsePipeline>(mainDevice, "Asset/Shader/vertex.spv", "Asset/Shader/fragment.spv", pipelineConfig);
	}

	void app::createCommandBuffers(){
		commandBuffers.resize(mainSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = mainDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(mainDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
	}

	void app::freeCommandBuffes(){
		vkFreeCommandBuffers(mainDevice.device(), mainDevice.getCommandPool(), static_cast<float>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void app::drawFrame(){
		uint32_t imageIndex;
		auto result = mainSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		recordCommandBuffer(imageIndex);
		result = mainSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mainWindow.wasWindowResized()) {
			mainWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}

	}

	void app::recreateSwapChain() {
		auto extent = mainWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = mainWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(mainDevice.device());

		if (mainSwapChain == nullptr){
			mainSwapChain = nullptr; // added here to prevent crash
			mainSwapChain = std::make_unique<rseSwapChain>(mainDevice, extent);
		}
		else {
			mainSwapChain = nullptr; // added here to prevent crash (idk if this one will create an issue)
			mainSwapChain = std::make_unique<rseSwapChain>(mainDevice, extent, std::move(mainSwapChain));
			if (mainSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffes();
				createCommandBuffers();
			}
		}

		// if renderpass compatible do nothing else - 13:21 (sleep time)
		createPipeline();
	}

	void app::recordCommandBuffer(int imageIndex){
		//imageIndex	
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mainSwapChain->getRenderPass();
		renderPassInfo.framebuffer = mainSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = mainSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(mainSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(mainSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ { 0, 0 }, mainSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		mainPipeline->bind(commandBuffers[imageIndex]);
		//vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		mainModel->bind(commandBuffers[imageIndex]);
		mainModel->draw(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}

	}

}