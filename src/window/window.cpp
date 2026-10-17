#include "window.h"
#include <stdexcept>

RSE::Window::Window(int w, int h, std::string name){
	this->width = w;
	this->height = h;

	this->windowName = name;

	initWindow();
}

RSE::Window::~Window(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

void RSE::Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}

void RSE::Window::initWindow(){
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
}

void RSE::Window::framebufferResizedCallback(GLFWwindow* window, int width, int height) {
	auto rseWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	rseWindow->frameBufferResized = true;
	rseWindow->width = width;
	rseWindow->height = height;
}