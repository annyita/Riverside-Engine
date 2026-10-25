#version 460

layout(location = 0) in vec4 crntPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;
layout(location = 3) in vec2 uv;
layout(location = 4) in vec3 tangents;
layout(location = 5) in vec3 biTangents;

layout (location = 0) out vec4 outColour;

layout(set = 0, binding = 0) uniform GlobalUBO {
		mat4 cameraMatrix;
		vec3 lightDir;
} ubo;

layout(push_constant) uniform Push{
	vec3 position;
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

void main(){

	vec3 nNormal = normalize(normal);
	vec3 nTangent = normalize(tangents);
	vec3 nBitangent = normalize(biTangents);

	// make the tbn 
	vec3 normalTex = normalize(vec3(0.5, 0.5, 1.0) * 2.0f - 1.0f);
	mat3 nTBN = mat3(nTangent, nBitangent, nNormal);
	vec3 newNormal = normalize(nTBN * normalTex);

	//	vec3 lightDirection = vec3(1.0f, 1.0f, 0.0f);
    float diffuse = max(dot(newNormal, ubo.lightDir), 0.0f);

	//outColour = vec4(uv, 0.0, 1.0);
	//outColour = vec4(newNormal, 1.0);
	outColour = vec4(colour * (diffuse + 0.05), 1.0);
}