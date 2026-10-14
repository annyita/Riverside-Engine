#include <rceApp.h>

void RSE::app::run(){

	while (!mainWindow.shouldClose()){
		glfwPollEvents();
	}
}
