#pragma once

#include "camera.h"
#include <GLFW/glfw3.h>

namespace RSEGAME {
	class game{
	public:

		static Camera camera;

		static float offset;

		static void update();
		static void renderUpdate(GLFWwindow* window, float deltatime, int width, int height);

	private:
		static constexpr float threshold = 1.0 / 1.0;
		static float accumulator;
	};
}