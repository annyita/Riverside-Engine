#include "game.h"
#include <util/rseTime.h>
#include <glm/glm.hpp>

namespace RSEGAME {

	Camera game::camera {800, 600, glm::vec3(0.0f)};
	float game::offset = 0.0f;
	float game::accumulator = 0.0f;

	void game::update() {

		offset += 0.1 * RSE::rseTime::mainTimer.deltatime;
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