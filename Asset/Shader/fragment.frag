#version 460

layout (location = 0) in vec3 vertexColour;
layout (location = 0) out vec4 outColour;

void main(){
	//outColour = vec4(1.0, 1.0, 0.0, 1.0);
	outColour = vec4(vertexColour, 1.0);
}