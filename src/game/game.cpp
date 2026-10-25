#include "game.h"
#include <util/rseTime.h>
#include <glm/glm.hpp>

#include "scene/entityHandler.h"

namespace RSEGAME {

	Camera game::camera {800, 600, glm::vec3(0.0f)};
	float game::offset = 0.0f;
	float game::accumulator = 0.0f;

	void game::update() {

		offset += 5.0 * RSE::rseTime::mainTimer.deltatime;

		//RSE::entityHandler::transform(glm::vec3(-4.0f, glm::sin(offset) * 0.1, 0.0f), glm::vec3(0.0f, offset, 0.0f), glm::vec3(1.0f), 0);
		//RSE::entityHandler::transform(glm::vec3(0.0f, glm::sin(offset) * 0.1, 0.0f), glm::vec3(offset, 0.0f, 0.0f), glm::vec3(1.0f), 1);
		//RSE::entityHandler::transform(glm::vec3(4.0f, glm::sin(offset) * 0.1, 0.0f), glm::vec3(0.0f, offset, 0.0f), glm::vec3(1.0f), 2);

		/*
		accumulator += RSE::rseTime::mainTimer.deltatime;

		if (accumulator > threshold) {

			accumulator = 0.0f;
		}
		*/

	}
	void game::renderUpdate(GLFWwindow* window, float deltatime, int width, int height){
		camera.SetViewportSize(width, height);
		camera.inputsMouse(window);
		camera.Inputs(window, deltatime);
		camera.updateMatrix();
	}

}