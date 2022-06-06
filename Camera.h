#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
	public:
		// Stores the main three vectors of the camera
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

		// Prevents the camera from jumping around when first clicking left mouse button
		bool firstClick = true;

		// Stores the width and the height of the screen
		int width;
		int height;

		// Adjust the speed of the camera and its sensitivity when looking around
		float speed = 0.1f;
		float sensitivity = 100.0f;

		// Camera constructor to set up initial value
		Camera(int width, int height, glm::vec3 position);

		// Updates and exports the camera Matrix to the Vertex Shader
		void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
		// Handles camera inputs
		void Inputs(GLFWwindow* window);
};



#endif