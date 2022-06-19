// This is my implementation of the following tutorial
// https://www.youtube.com/watch?v=45MIykWJ-C4



#include "Model.h"

const unsigned int width = 1600;
const unsigned int height = 1600;




int main()
{
	glfwInit();

	// tell GLFW what version of OpenGL we are using
	// We are using version 3.3

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell OpenGL we are using the core profile, as opposed to the compatibility profile
	// In this profile we only have access to the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window of 800 by 600 pixels (width, height, name, fullscreen, idk)
	GLFWwindow* window = glfwCreateWindow(width, height, "Did they know we were coming?", NULL, NULL);

	// Catch the error if for some reason the window failed to create.
	if (window == NULL)																						
	{	
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Bring the window we just created into the current context

	glfwMakeContextCurrent(window);																			

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);







	//Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f); 
	lightModel = glm::translate(lightModel, lightPos);

	
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);






	//Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);


	//Create camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model("Resources/models/sword/scene.gltf");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera Matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 300.0f);


		// Draw a model
		model.Draw(shaderProgram, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window at the end of the main
	glfwDestroyWindow(window);		
	// Exit GLFW before leaving main
	glfwTerminate();
	return 0;
}