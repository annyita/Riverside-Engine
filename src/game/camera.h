#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace RSEGAME {

	class Camera
	{
	public:
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 invView = glm::mat4(1.0f);
		glm::mat4 invProjection = glm::mat4(1.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		glm::vec3 position = glm::vec3(0.0f);
		float near = 0.1f;
		float far = 500.0f;
		float fov = 75.0f;
		float gamma = 2.2f;
		float speed = 0.1f;
		bool doingWASD = false;
		bool doViewBob = true;
		float bob = 0.0f;

		Camera(int width, int height, glm::vec3 position);

		void SetViewportSize(int newWidth, int newHeight);

		void updateMatrix();

		void inputsMouse(GLFWwindow* window);

		void Inputs(GLFWwindow* window, float deltatime);

	private:
		bool MouseState = true, toggleESC = true; bool firstClick = false;
		float s_scrollSpeed = 0.0f;
		int width = 0;
		int height = 0;

		float bobAccumulate = 0.0f;
		float bobIntensity = 0.0f;

		float aspect = 0.0f;
		glm::vec2 sensitivity = glm::vec2(100.0f);

		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f); // forward vector
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(0.0f, 0.0f, 0.0f);
	};
}