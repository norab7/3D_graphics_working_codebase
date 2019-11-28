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

layout(binding=0) uniform sampler2D tex;
layout(binding=1) uniform sampler2D tex2;

uniform float count;
uniform float trans;

void main(void){
	vec3 normalsT = mat3(transpose(inverse(model_matrix))) * vec3(fs_in.normals.xyz);
	vec4 normal = normalize(vec4(normalsT, 1.0));
	vec4 lightDir = normalize(lightPosition - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = diff * vec4(0.5, 0.5,0.5, 1.0);
	
	// color = vec4( ka * ia , 1.0) + diffuse;
	// color = texture(tex, fs_in.tc);

	color = texture(tex, fs_in.tc) * ( vec4( ka * ia, 1.0) + diffuse );
	// color = mix(texture(tex, fs_in.tc), texture(tex2, fs_in.tc), 0.2) + diffuse;

	/*
	if( count < 10 ){
		// color = vec4(1,0,0,1);
		color = (texture(tex, fs_in.tc) + vec4( ka * ia , 1.0)) + diffuse;
		color = mix(texture(tex, fs_in.tc), vec4( ka * ia , 1.0), 0.2) + diffuse;

	} else if (count >= 10 && count < 20) {
		// color = vec4(0,1,0,1);
		color = texture(tex2, fs_in.tc) + diffuse;
	} else {
		// color = vec4(0,0,1,0.5);

		color = mix(mix(texture(tex, fs_in.tc), vec4( ka * ia , 1.0), 0.2) + diffuse, vec4(0,0,1,1), trans);
	}
	*/
}