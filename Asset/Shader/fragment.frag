#version 460

layout (location = 0) in vec2 uv;
layout (location = 1) in vec4 crntPos;

layout (location = 0) out vec4 outColour;


layout(push_constant) uniform Push{
	vec3 position;
    mat4 cameraMatrix;
	mat4 modelMatrix;
} push;

void main(){
	outColour = vec4(uv, 0.0, 1.0);
}