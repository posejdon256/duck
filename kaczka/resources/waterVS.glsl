#version 330 core
#extension GL_ARB_explicit_uniform_location : require


layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;
layout(location = 14) uniform vec3 cameraPostion;
layout(location = 15) uniform float time;

out vec3 FragPos; 
out vec3 Normal;
out vec3 Cam;
out vec3 TexCoords;
out vec3 TexCoords2;
out vec3 ab_pos;

void main() {
	gl_Position = projection * view * model  * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = vec3(model * vec4(normal, 1.0));;
	Cam = cameraPostion;
	TexCoords = vec3(position.x + 0.5f, 0.0f, position.z + 0.5f);
	TexCoords2 = 2 * position;
	ab_pos = Normal.y < 0.0f ? vec3(- (position.x + 0.5f), 0.0f, - (position.z + 0.5f)) : TexCoords;
}
