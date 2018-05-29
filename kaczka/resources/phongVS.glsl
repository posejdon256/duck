#version 330 core
#extension GL_ARB_explicit_uniform_location : require


layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;

out vec3 TexCoord;

void main() {
    gl_Position =  projection * view * model  * vec4(position, 1.0);
	vec3 FragPos = vec3(model * vec4(position, 1.0));
	vec3 help = vec3(position.x + 0.5f, 0.0f, position.z + 0.5f);
	vec3 helpFragPos = vec3(model * vec4(help, 1.0));
	TexCoord = FragPos.xyz;//vec2(help.x + 0.5f, help.z + 0.5f);
}
