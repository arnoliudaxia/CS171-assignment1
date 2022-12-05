#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Ouput data
out vec3 color;

void main(){

	//color = vec3(1,0.247,0.184);
	color=fragmentColor;

}