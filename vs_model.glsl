#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 tc;
layout (location = 2) in vec4 normals;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(void){
	FragPos = vec3(model * position);	
	Normal = mat3(transpose(inverse(model))) * vec3(normals.xyz);
	TexCoords = tc;
	
	gl_Position = proj * view * model  * position;
}