#include "simpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <array>
#include <util/rseTime.h>

#include <game/game.h>

#include <util/rseMath.h>

#include <render/object/rseMesh.h>
#include <game/scene/entityHandler.h>


namespace RSE {

	struct simplePushConstantData {
		alignas(16) glm::vec3 position;
		alignas(16) glm::mat4 cameraMatrix;
		alignas(16) glm::mat4 modelMatrix;
		alignas(16) glm::mat4 normalMatrix;
	};

	simpleRenderSystem::simpleRenderSystem(rseDevice& device, VkRenderPass renderPass) : mainDevice(device) {
		createPipelineLayout();
		createPipeline(renderPass);
	}

	simpleRenderSystem::~simpleRenderSystem(){
		vkDestroyPipelineLayout(mainDevice.device(), pipelineLayout, nullptr);
	}

	void simpleRenderSystem::recreateRenderSystem(VkRenderPass renderPass){
		vkDestroyPipelineLayout(mainDevice.device(), pipelineLayout, nullptr);
		createPipelineLayout();
		createPipeline(renderPass);
	}

	void simpleRenderSystem::renderEntities(VkCommandBuffer commandBuffer){
		mainPipeline->bind(commandBuffer);

		for (auto& entityHandler : entityHandler::entities) {

			if (!entityHandler.render.doRender) continue;

			switch (entityHandler.render.renderType){
				case entityHandler::type_mesh:
					renderMesh(commandBuffer, entityHandler);
				break;
			default:
				case entityHandler::type_model:
					renderModel(commandBuffer, entityHandler);
				break;
			}
		}
	}

	void simpleRenderSystem::renderMesh(VkCommandBuffer commandBuffer, auto& entityHandler){
		simplePushConstantData push{};
		push.position = RSEGAME::game::camera.position + glm::vec3(0.0f, RSEGAME::game::camera.bob, 0.0f);
		push.cameraMatrix = RSEGAME::game::camera.cameraMatrix;
		push.modelMatrix = entityHandler.transform.matrix;
		push.normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(push.modelMatrix))));

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

	void simpleRenderSystem::renderModel(VkCommandBuffer commandBuffer, auto& entityHandler){

		for (size_t i = 0; i < entityHandler.render.model->meshes.size(); i++){
			simplePushConstantData push{};
			push.position = RSEGAME::game::camera.position + glm::vec3(0.0f, RSEGAME::game::camera.bob, 0.0f);
			push.cameraMatrix = RSEGAME::game::camera.cameraMatrix;
			push.modelMatrix = entityHandler.render.model->localTransformations[i].matrix * entityHandler.transform.matrix;
			push.normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(push.modelMatrix))));

			vkCmdPushConstants(commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(simplePushConstantData),
				&push
			);

			entityHandler.render.model->meshes[i]->bind(commandBuffer);
			entityHandler.render.model->meshes[i]->draw(commandBuffer);
		}
	}

	void simpleRenderSystem::createPipelineLayout(){
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

	void simpleRenderSystem::createPipeline(VkRenderPass renderPass){
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		pipelineCfgInfo pipelineConfig{};
		rsePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		mainPipeline = std::make_unique<rsePipeline>(mainDevice, "Asset/Shader/vertex.spv", "Asset/Shader/fragment.spv", pipelineConfig);
	}
}