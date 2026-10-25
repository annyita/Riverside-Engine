#include <rseApp.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <array>
#include <util/rseTime.h>

#include <game/game.h>

#include <util/rseMath.h>

#include <render/simpleRenderSystem.h>

#include <render/object/rseMesh.h>

#include <render/vkCore/rseBuffer.h>

namespace RSE {

	struct globalUBO{
		glm::mat4 cameraMatrix;
		glm::vec3 lightDir = glm::vec3(1.0, 1.0, 0.0);
	};

	app::app() {
		globalPool = rseDescriptorPool::Builder(mainDevice)
			.setMaxSets(rseSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, rseSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();


		loadEntities();

		mainWindow.setWindowIcon("Asset/Dependants/icon.png");
	}
	app::~app() {
		entityHandler::destroy();
	}

	void app::run() {

		std::vector<std::unique_ptr<rseBuffer>> uboBuffers(rseSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (size_t i = 0; i < uboBuffers.size(); i++){
			uboBuffers[i] = std::make_unique<rseBuffer>(
				mainDevice,
				sizeof(globalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = rseDescriptorSetLayout::Builder(mainDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(rseSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (size_t i = 0; i < globalDescriptorSets.size(); i++){
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			rseDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		simpleRenderSystem simpleRenderSystem{ mainDevice, mainRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };

		while (!mainWindow.shouldClose()) {
			glfwPollEvents();
			rseTime::updateTimer(rseTime::mainTimer);
			RSEGAME::game::renderUpdate(mainWindow.getWindowPointer(), rseTime::mainTimer.deltatime, width, height);
			RSEGAME::game::update();

			if (auto commandBuffer = mainRenderer.beginFrame()) {

				globalUBO ubo{};
				ubo.cameraMatrix = RSEGAME::game::camera.cameraMatrix;
				ubo.lightDir = glm::normalize(glm::vec3(1.0, 1.0, 0.0));
				uboBuffers[mainRenderer.getFrameIndex()]->writeToBuffer(&ubo);
				uboBuffers[mainRenderer.getFrameIndex()]->flush();
					
				mainRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderEntities(commandBuffer, globalDescriptorSets[mainRenderer.getFrameIndex()]);
				mainRenderer.endSwapChainRenderPass(commandBuffer);
				mainRenderer.endFrame();
			}

			// restarting the render pipeline
			if (glfwGetKey(mainWindow.getWindowPointer(), GLFW_KEY_F3) == GLFW_PRESS && 
				glfwGetKey(mainWindow.getWindowPointer(), GLFW_KEY_R) == GLFW_PRESS) {
				simpleRenderSystem.recreateRenderSystem(mainRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
			}
		}
	}

	void app::loadEntities() {


		//auto newModel = std::make_shared<Model>("Asset/Model/trans robot thingy/trans_robot.gltf", mainDevice);
		auto newModel = std::make_shared<Model>("Asset/Model/bistro/Bistro.gltf", mainDevice);

		/*
		entityHandler::entity newEntity;
		newEntity.render.model = std::make_shared<Model>("Asset/Model/basic shapes 2/cube.gltf", mainDevice);
		newEntity.render.renderType = entityHandler::type_model;
		newEntity.render.doRender = true;
		
		entityHandler::entity newEntity2;
		newEntity2.render.model = std::make_shared<Model>("Asset/Model/basic shapes 2/sphere.gltf", mainDevice);
		newEntity2.render.renderType = entityHandler::type_model;
		newEntity2.render.doRender = true;

		entityHandler::entity newEntity3;
		newEntity3.render.model = std::make_shared<Model>("Asset/Model/basic shapes/torus.gltf", mainDevice);
		newEntity3.render.renderType = entityHandler::type_model;
		newEntity3.render.doRender = true;
		*/

		entityHandler::entity newEntity4;
		newEntity4.render.model = newModel;
		newEntity4.render.renderType = entityHandler::type_model;
		newEntity4.render.doRender = true;

		newEntity4.transform.position = glm::vec3(10.0f, -5.0f, 0.0f);
		newEntity4.transform.rotation = glm::vec3(0.0f, -75.0f, 0.0f);
		
		//entityHandler::create(newEntity);
		//entityHandler::create(newEntity2);
		//entityHandler::create(newEntity3);
		entityHandler::create(newEntity4);
		entityHandler::transform(entityHandler::entities.size() - 1);
		
	}
}