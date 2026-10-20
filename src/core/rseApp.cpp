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

namespace RSE {

	app::app() {
		loadEntities();


		mainWindow.setWindowIcon("Asset/Dependants/icon.png");
	}
	app::~app() {

	}

	void app::run() {

		simpleRenderSystem simpleRenderSystem{ mainDevice, mainRenderer.getSwapChainRenderPass() };

		while (!mainWindow.shouldClose()) {
			glfwPollEvents();
			rseTime::updateTimer(rseTime::mainTimer);
			RSEGAME::game::renderUpdate(mainWindow.getWindowPointer(), rseTime::mainTimer.deltatime, width, height);
			RSEGAME::game::update();

			if (auto commandBuffer = mainRenderer.beginFrame()) {
				mainRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderEntities(commandBuffer);
				mainRenderer.endSwapChainRenderPass(commandBuffer);
				mainRenderer.endFrame();
			}
		}
	}

	void app::loadEntities() {
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
}