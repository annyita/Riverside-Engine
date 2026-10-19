#version 460

layout(location = 0) in vec3 aPositon;
layout(location = 1) in vec2 aUV;

layout(location = 0) out vec2 uv;
layout(location = 1) out vec4 crntPos;

layout(push_constant) uniform Push{
	vec3 positon;
	mat4 cameraMatrix;
	mat4 modelMatrix;
} push;

void main(){ //  + push.offset
	vec4 ncp = push.modelMatrix * vec4(aPositon, 1.0);
	crntPos = ncp;
	uv = aUV;

	gl_Position = push.cameraMatrix * ncp;
}
	