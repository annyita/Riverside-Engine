#version 460

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColour;
layout(location = 3) in vec2 aUV;
layout(location = 4) in vec3 aTangents;
layout(location = 5) in vec3 aBiTangents;

layout(location = 0) out vec4 crntPos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec3 colour;
layout(location = 3) out vec2 uv;
layout(location = 4) out vec3 tangents;
layout(location = 5) out vec3 biTangents;

layout(set = 0, binding = 0) uniform GlobalUBO {
		mat4 cameraMatrix;
		vec3 lightDir;
} ubo;

layout(push_constant) uniform Push{
	vec3 position;
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;


uvec3 murmurHash31(uint src) {
    const uint M = 0x5bd1e995u;
    uvec3 h = uvec3(1190494759u, 2147483647u, 3559788179u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 3 outputs, 1 input
vec3 hash31(float src) {
    uvec3 h = murmurHash31(floatBitsToUint(src));
    return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

void main(){ //  + push.offset
	vec4 ncp = push.modelMatrix * vec4(aPosition, 1.0);
	crntPos = ncp;
	uv = aUV;
	colour = aColour;
	//colour = hash31(float(gl_VertexIndex));

	//normal = normalize(aNormal);
	mat3 normalMatrix = mat3(push.normalMatrix);

	normal = normalMatrix * aNormal;
    tangents = normalMatrix * aTangents;
    biTangents = normalMatrix * aBiTangents;

	gl_Position = ubo.cameraMatrix * ncp;
}
	