#include <rseApp.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <array>
#include <util/rseTime.h>

#include <game/game.h>

#include <util/rseMath.h>

namespace RSE {

	struct simplePushConstantData {
		alignas(16) glm::vec3 position;
		alignas(16) glm::mat4 cameraMatrix;
		alignas(16) glm::mat4 modelMatrix;
	};

	app::app() {
		loadEntities();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();

		mainWindow.setWindowIcon("Asset/Dependants/icon.png");
	}
	app::~app(){
		vkDestroyPipelineLayout(mainDevice.device(), pipelineLayout, nullptr);
	}
	
	void app::run() {

		while (!mainWindow.shouldClose()) {
			glfwPollEvents();
			rseTime::updateTimer(rseTime::mainTimer);
			RSEGAME::game::renderUpdate(mainWindow.getWindowPointer(), rseTime::mainTimer.deltatime, width, height);
			RSEGAME::game::update();
			drawFrame();
		}
	}

	void app::loadEntities(){
		std::vector<Mesh::vertex> vertices{
			{ { 0.0f, -0.5f, 0.0f },{ 1.0, 0.0 } },
			{ { 0.5f, 0.5f, 0.0f },{ 0.0, 1.0 } },
			{ { -0.5f, 0.5f, 0.0f },{ 0.0, 0.0 } },
		};

		std::vector<Mesh::vertex> vertices2{

			// front
			{ { -1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, -1.0f, 1.0f },{ 1.0f, 0.0f } },
			{ { 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f,  1.0f, 1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f } },


			// back
			{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { 1.0f,  1.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f,  1.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f } },


			// left
			{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { -1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f } },
			{ { -1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f } },

			// right
			{ { 1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { 1.0f,  1.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f,  1.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f } },
			{ { 1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f } },


			// bottom
			{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, -1.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { 1.0f, -1.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f, -1.0f,  1.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f } },

			// top
			{ { -1.0f, 1.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, 1.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { 1.0f, 1.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f, 1.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f, 1.0f,  1.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, 1.0f, -1.0f },{ 0.0f, 0.0f } }
		};

		std::vector<Mesh::vertex> vertices3{

			// front
			{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, -1.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f } },

			// right
			{ { 0.0f, -1.0f,  1.0f },{ 0.0f, 0.0f } },
			{ { 0.0f, -1.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { 0.0f,  1.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { 0.0f,  1.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { 0.0f,  1.0f,  1.0f },{ 0.0f, 1.0f } },
			{ { 0.0f, -1.0f,  1.0f },{ 0.0f, 0.0f } },

			// top
			{ { -1.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { 1.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { 1.0f, 0.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f, 0.0f,  1.0f },{ 1.0f, 1.0f } },
			{ { -1.0f, 0.0f,  1.0f },{ 0.0f, 1.0f } },
			{ { -1.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } }
		};

		auto model = std::make_shared<Mesh>(mainDevice, vertices);

		auto model2 = std::make_shared<Mesh>(mainDevice, vertices2);

		auto model3 = std::make_shared<Mesh>(mainDevice, vertices3);

		entityHandler::entity newEntity;
		newEntity.render.mesh = model;

		entityHandler::entity newEntity2;
		newEntity2.render.mesh = model2;

		entityHandler::entity newEntity3;
		newEntity3.render.mesh = model3;

		entityHandler::create(newEntity);
		entityHandler::create(newEntity2);
		entityHandler::create(newEntity3);
	}

	void app::createPipelineLayout() {
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(simplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(mainDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout");
		}

	}

	void app::createPipeline(){
		pipelineCfgInfo pipelineConfig{};
		rsePipeline::defaultPipelineConfigInfo(pipelineConfig);
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
		vkFreeCommandBuffers(mainDevice.device(), mainDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
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
		clearValues[0].color = { 0.3f, 0.3f, 0.9f, 1.0f };
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

		renderEntities(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}

	}

	void app::renderEntities(VkCommandBuffer commandBuffer){
		mainPipeline->bind(commandBuffer);

		for (auto& entityHandler : entityHandler::entities) {

			simplePushConstantData push{};
			push.position = RSEGAME::game::camera.position + glm::vec3(0.0f, RSEGAME::game::camera.bob, 0.0f);
			push.cameraMatrix = RSEGAME::game::camera.cameraMatrix;
			push.modelMatrix = entityHandler.transform.matrix;

			vkCmdPushConstants(commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(simplePushConstantData),
				&push
			);

			entityHandler.render.mesh->bind(commandBuffer);
			entityHandler.render.mesh->draw(commandBuffer);
		}
	}

}