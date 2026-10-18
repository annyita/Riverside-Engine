#include "window.h"
#include <stdexcept>

// probably temporary
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace RSE {

	Window::Window(int w, int h, std::string name) {
		this->width = w;
		this->height = h;

		this->windowName = name;

		initWindow();
	}

	Window::~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Window::setWindowIcon(std::string path) {
		int iconW, iconH;
		int iconChannels;
		stbi_set_flip_vertically_on_load(false);

		unsigned char* pixelsIcon = stbi_load((path).c_str(), &iconW, &iconH, &iconChannels, STBI_rgb_alpha);

		GLFWimage Iconinages[1];
		Iconinages[0].width = iconW;
		Iconinages[0].height = iconH;
		Iconinages[0].pixels = pixelsIcon;

		glfwSetWindowIcon(window, 1, Iconinages);
	}

	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void Window::initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
	}

	void Window::framebufferResizedCallback(GLFWwindow* window, int width, int height) {
		auto rseWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		rseWindow->frameBufferResized = true;
		rseWindow->width = width;
		rseWindow->height = height;
	}
}