#version 330 core

in vec3 Normal;
in vec3 FragPos;  
out vec4 outColor;

vec3 objectColor;
vec3 lightColor;
in vec2 TexCoord;
in vec3 camera;

uniform sampler2D ourTexture;

vec3 lightPos;
vec3 lightPos2;

void main() {
	lightColor = vec3(1.0f,1.0f,1.0f);
	objectColor = vec3(0.4f, 0.4f, 0.4f);
	lightPos = vec3(4.0f, 4.0f, 4.0f);
	lightPos2 = vec3(4.0f, 4.0f, 4.0f);

	float specularStrength = 0.7f;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 

	vec3 viewDir = normalize(lightPos2 - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	vec3 toLight = normalize(lightPos - FragPos);
	vec3 toCamera = normalize(camera - FragPos);
	vec3 H = normalize(toLight + toCamera);
	vec3 T = vec3(norm.x, -norm.z, norm.y);

	float _cos = sqrt(1 - pow(dot(H, T), 2));
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(_cos, 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;  

	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse + specular) *  vec3(texture(ourTexture, TexCoord));
	outColor = vec4(result, 1.0f);

}
