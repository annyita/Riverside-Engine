#ifndef RIVERSIDE_WINDOW_CLASS_H
#define RIVERSIDE_WINDOW_CLASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace RSE {

	class Window {
	public:

		Window(int w, int h, std::string name);
		~Window();

		bool shouldClose() { return glfwWindowShouldClose(window); }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	private:

		void initWindow();
		int width;
		int height;

		std::string	windowName;

		GLFWwindow* window;

	};
}

#endif