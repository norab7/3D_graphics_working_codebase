#version 450 core

out vec4 color;

in VS_OUT{
	vec2 tc;
	vec4 normals;
	vec4 fragPos;
} fs_in;

uniform mat4 model_matrix;
uniform vec3 materialColor;
uniform vec4 lightPosition;
uniform vec3 ia; // Ambient colour
uniform float ka; // Ambient constant

void main(void){
	vec3 normalsT = mat3(transpose(inverse(model_matrix))) * vec3(fs_in.normals.xyz);
	vec4 normal = normalize(vec4(normalsT, 1.0));
	vec4 lightDir = normalize(lightPosition - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = diff * vec4(1.0, 1.0, 1.0, 1.0);
	color = vec4( ka * ia , 1.0) + diffuse;
}