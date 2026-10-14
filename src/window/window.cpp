#include "window.h"

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

void RSE::Window::initWindow(){
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
