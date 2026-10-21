#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace RSEGAME {

	Camera::Camera(int width, int height, glm::vec3 position) {
		this->width = width;
		this->height = height;
		this->position = position;
	}

	void Camera::SetViewportSize(int newWidth, int newHeight) {
		this->width = newWidth;
		this->height = newHeight;

		this->aspect = static_cast<float>(width) / static_cast<float>(height);
	}

	void Camera::updateMatrix(){
		// Makes camera look in the right direction from the right position
        view = glm::lookAt(position, position + Orientation, Up);
        Right = glm::normalize(glm::cross(Orientation, Up));

        aspect = static_cast<float>(width) / static_cast<float>(height);

        projection = glm::perspective(glm::radians(fov), aspect, near, far);
        projection[1][1] *= -1.0f;

        cameraMatrix = projection * view;
	
        // inv matrices
        invProjection = glm::inverse(projection);
        invView = glm::inverse(view);
    }

	void Camera::inputsMouse(GLFWwindow* window){
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (toggleESC) { 
                MouseState = !MouseState;
                toggleESC = false;
            }

        }
        else{
            toggleESC = true;
        }
        // Handles mouse inputs
        if (MouseState){
            // Disables mouse cursor and prevents it from leaving the window
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            // Prevents camera from jumping on the first click
            if (firstClick){
                glfwSetCursorPos(window, (width / 2), (height / 2));
                firstClick = false;
            }


            // Stores the coordinates of the cursor
            //double mouseX =  windowHandler::mouseX;
            //double mouseY =  windowHandler::mouseY;
            // Fetches the coordinates of the cursor

            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
            // and then "transforms" them into degrees 
            float rotX = Camera::sensitivity.y * (float)(mouseY - (height / 2)) / height;
            float rotY = Camera::sensitivity.x * (float)(mouseX - (width / 2)) / width;

            // Calculates upcoming vertical change in the Orientation
            glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

            // Decides whether or not the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)){
                Orientation = newOrientation;
            }

            // Rotates the Orientation left and right
            Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (width / 2), (height / 2));
        }
        else{
            // Unhides cursor since camera is not looking around anymore
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // Makes sure the next time the camera looks around it doesn't jump
            firstClick = true;
        }
	}

    void Camera::Inputs(GLFWwindow* window, float deltatime){
        float adjustedSpeed = speed * deltatime;

        doingWASD = false;

        // wasd
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += adjustedSpeed * Orientation;                                    doingWASD = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position += adjustedSpeed * -glm::normalize(glm::cross(Orientation, Up));   doingWASD = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position += adjustedSpeed * -Orientation;                                   doingWASD = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position += adjustedSpeed * glm::normalize(glm::cross(Orientation, Up));    doingWASD = true;
        }

        if (doViewBob) {

            if (doingWASD) {
                bobAccumulate += 15.0 * deltatime;
                bobIntensity = 1.0f;
            }
            else bobIntensity = glm::max(bobIntensity - 3.0f * deltatime, 0.0f);

            bob = sin(bobAccumulate) * (0.015 * bobIntensity);
        }

        // up and down
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //jump
            position += adjustedSpeed * Up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            position += adjustedSpeed * -Up;


        // sprint and flight speed
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            s_scrollSpeed = glm::max(s_scrollSpeed + 10.0f * deltatime, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            s_scrollSpeed = glm::max(s_scrollSpeed - 10.0f * deltatime, 0.0f);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            speed = (10.0f + s_scrollSpeed);
        else speed = (5.0f + s_scrollSpeed);
    }

}