#version 460

layout(location = 0) in vec2 positon;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 vertexColor;

vec3 hash32(vec2 p){
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+33.33);
    return fract((p3.xxy+p3.yzz)*p3.zyx);
}

void main(){
	gl_Position = vec4(positon, 0.0, 1.0);
	//vertexColor = hash32(positon);
	vertexColor = colour;
}
	