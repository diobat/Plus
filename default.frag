#version 330 core

// Outputs the colors in RGBA
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the texture unit from the main function
uniform sampler2D tex0;

void main()
{
	//FragColor = vec4(color, 1.0f);

	FragColor = texture(tex0, texCoord) * vec4(color, 0.5f);
}