// This is my implementation of the following tutorial
// https://www.youtube.com/watch?v=45MIykWJ-C4



#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"





	// Vertices coordinates 
GLfloat vertices[] =
{//				COORDINATES						/			COLORS
	-0.5f,	-0.5f * float(sqrt(3)) / 3,		0.0f,	0.8f,	0.3f,	0.02f,	// Lower left corner
	 0.5f,	-0.5f * float(sqrt(3)) / 3,		0.0f,	0.8f,	0.3f,	0.02f,	// Lower right corner
	 0.0f,	 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	1.0f,	0.6f,	0.32f,  // Upper corner 
	-0.25f,  0.5f * float(sqrt(3)) / 6,		0.0f,	0.9f,	0.45f,	0.17f,	// Inner left
	 0.25f,	 0.5f * float(sqrt(3)) / 6,		0.0f,	0.9f,	0.45f,	0.17f,	// Inner right
	 0.0f,	-0.5f * float(sqrt(3)) / 3,		0.0f,	0.8f,	0.3f,	0.02f	// Inner down
};


GLuint indices[] =
{
	0, 3, 5,		// Lower left triangle
	3, 2, 4,		// Lower right triangle
	5, 4, 1			// Upper triangle
};




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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Did they know we were coming?", NULL, NULL);		

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
	glViewport(0, 0, 800, 800);





	//Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	//Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();


	// Generate Vertex Buffer Object and  link it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer Object and link it to indices
	EBO EBO1(indices, sizeof(indices));


	// Link VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0) ;
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 *  sizeof(float)));


	//Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		//Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);


		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);


		// Take care of all GLFW events
		glfwPollEvents();

	}

	// Delete all the objects we've created

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Delete window at the end of the main
	glfwDestroyWindow(window);		
	// Exit GLFW before leaving main
	glfwTerminate();
	return 0;
}