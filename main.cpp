// This is my implementation of the following tutorial
// https://www.youtube.com/watch?v=45MIykWJ-C4



#include "Model.h"

const unsigned int width = 1600;
const unsigned int height = 1600;


// Takes care of the information needed to draw the windows
const unsigned int numWindows = 100;
glm::vec3 positionsWin[numWindows];
float rotationsWin[numWindows];

// Takes care of drawing the windows in the right order
unsigned int orderDraw[numWindows];
float distanceCamera[numWindows];

// Compare function
int compare(const void* a, const void* b)
{
	double diff = distanceCamera[*(int*)b] - distanceCamera[*(int*)a];
	return  (0 < diff) - (diff < 0);
}

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
	Shader grassProgram("default.vert", "grass.frag");
	Shader winProgram("default.vert", "windows.frag");



	// Take care of all light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f); 
	lightModel = glm::translate(lightModel, lightPos);

	
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	grassProgram.Activate();
	glUniform4f(glGetUniformLocation(grassProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(grassProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	//Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Create camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	// Load in the models from .gltf files
	Model ground("Resources/models/ground2/scene.gltf");
	Model grass("Resources/models/grass/scene.gltf");
	Model windows("Resources/models/windows/scene.gltf");


	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;



	// Disable V-Sync (not advised)
	//glfwSwapInterval(0);


	for (unsigned int i = 0; i < numWindows; i++)
	{
		positionsWin[i] = glm::vec3
		(
			-15.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15.0f - (-15.0f)))),
			1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4.0f - 1.0f))),
			-15.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15.0f - (-15.0f))))
		);
		rotationsWin[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		orderDraw[i] = i;
	}



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "OpenGL - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;

			// Handles camera inputs
			// This function is inside this if so it gets called a fixed
			// number of times per second and camera speed doesnt get tied
			// with FPS rate
			// Comment if vsync is enabled
			//camera.Inputs(window);

		}


		// Specify the color of the background
		glClearColor(0.15f, 0.15f, 0.10f, 1.0f);
		// Clean the back buffer and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		// Comment if vsync is disabled
		camera.Inputs(window);
		// Updates and exports the camera Matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		// Draw a model

		ground.Draw(shaderProgram, camera);
		glDisable(GL_CULL_FACE);
		grass.Draw(grassProgram, camera);
		glEnable(GL_BLEND);
		for (unsigned int i = 0; i < numWindows; i++)
		{
			distanceCamera[i] = glm::length(camera.Position - positionsWin[i]);
		}
		qsort(orderDraw, numWindows, sizeof(unsigned int), compare);
		for (unsigned int i = 0; i < numWindows; i++)
		{
			windows.Draw(winProgram, camera, positionsWin[orderDraw[i]], glm::quat(1.0f, 0.0f, rotationsWin[orderDraw[i]], 0.0f));
		}
		glDisable(GL_BLEND);

		glEnable(GL_CULL_FACE);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	grassProgram.Delete();
	// Delete window at the end of the main
	glfwDestroyWindow(window);		
	// Exit GLFW before leaving main
	glfwTerminate();
	return 0;
}