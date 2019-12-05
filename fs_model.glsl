#version 450 core

out vec4 color;

// Materials
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};


// Lights
struct D_light {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct P_light {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct S_light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


// Choose how many lights, Must Define
#define POINT_LIGHTS_COUNT 3
#define SPOT_LIGHTS_COUNT 5

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Stored Variables
uniform vec3 viewPos;
uniform D_light d_light;
uniform P_light p_light[POINT_LIGHTS_COUNT];
uniform S_light s_light[SPOT_LIGHTS_COUNT];
uniform Material material;

// Textures - TODO: fix having to load textures
uniform vec4 lightPosition0;
uniform vec4 lightPosition1;

uniform vec3 ia; // Ambient colour
uniform float ka; // Ambient constant

layout(binding=0) uniform sampler2D tex;



// Prototypes
vec3 calculate_d_light(D_light light, vec3 normal, vec3 view);
vec3 calculate_p_light(P_light light, vec3 normal, vec3 fragPos, vec3 view);
vec3 calculate_s_light(S_light light, vec3 normal, vec3 fragPos, vec3 view);



// Misc Variables
uniform float count;
uniform float spot_light_flash;
uniform float spot_light_active;

float alpha = (count == 6) ? 0.75 : 1.0;


void main(void){
	
	vec3 normal = normalize(Normal);
	vec3 view_direction = normalize(viewPos - FragPos);
	
	vec3 light_results = calculate_d_light(d_light, normal, view_direction);
	for(int i = 0; i < POINT_LIGHTS_COUNT; i++){
		light_results += calculate_p_light(p_light[i], normal, FragPos, view_direction);
	}

	// FLASHY LIGHTY
	for(int i = 0; i < SPOT_LIGHTS_COUNT; i++){
		if((spot_light_flash == 1.0 && spot_light_active == 1.0) || i > 0){ 
			light_results += calculate_s_light(s_light[i], normal, FragPos, view_direction);
		}
	}

	color = vec4(light_results, alpha);
}

// Directional Lights
vec3 calculate_d_light(D_light light, vec3 normal, vec3 view){
	vec3 light_direction = normalize(-light.direction);
	float diff = max(dot(normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(view, reflect_direction), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(tex, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(tex, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(tex, TexCoords));
	return (ambient + diffuse + specular);
}

// Point Lights
vec3 calculate_p_light(P_light light, vec3 normal, vec3 fragPos, vec3 view){
	vec3 light_direction = normalize(light.position - fragPos);
	float diff = max(dot(normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(view, reflect_direction), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(tex, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(tex, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(tex, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

// Spot Lights
vec3 calculate_s_light(S_light light, vec3 normal, vec3 fragPos, vec3 view){
	vec3 light_direction = normalize(light.position - fragPos);

	float diff = max(dot(normal, light_direction), 0.0);
	
	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(view, reflect_direction), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(light_direction, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.ambient * vec3(texture(tex, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(tex, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(tex, TexCoords));
	ambient *= attenuation  * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}