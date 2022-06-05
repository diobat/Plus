// This is my implementation of the following tutorial
// https://www.youtube.com/watch?v=45MIykWJ-C4



#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


const unsigned int width = 800;
const unsigned int height = 800;

	// Vertices coordinates 
GLfloat vertices[] =
{//	  COORDINATES		  /			COLORS				/	TexCoord		//
	-0.5f,	 0.0f,	0.5f,		0.83f,	0.70f,	0.44f,		0.0f,	0.0f,	
	-0.5f,	 0.0f, -0.5f,		0.83f,	0.70f,	0.44f,		5.0f,	0.0f,	
	 0.5f,	 0.0f, -0.5f,		0.83f,	0.70f,	0.44f,		0.0f,	0.0f,
	 0.5f,	 0.0f,	0.5f,		0.83f,	0.70f,	0.44f,		5.0f,	0.0f,
	 0.0f,	 0.8f,	0.0f,		0.92f,	0.86f,	0.76f,		2.5f,	5.0f

};


GLuint indices[] =
{
	0, 1, 2,	
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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

	//Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();


	// Generate Vertex Buffer Object and  link it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer Object and link it to indices
	EBO EBO1(indices, sizeof(indices));


	// Link VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0) ;
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 *  sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 *  sizeof(float)));


	//Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture

	Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);


	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	//Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();


		//Simple timer
		double crntTime = glfwGetTime();
		if(crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}


		//Initializes matrices so they are not the null matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);


		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);


		// Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		// Binds texture so that it appears in rendering
		popCat.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
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
	popCat.Delete();
	shaderProgram.Delete();

	// Delete window at the end of the main
	glfwDestroyWindow(window);		
	// Exit GLFW before leaving main
	glfwTerminate();
	return 0;
}