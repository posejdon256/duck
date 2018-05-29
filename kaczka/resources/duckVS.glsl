#version 330 core
#extension GL_ARB_explicit_uniform_location : require


layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 textures;
layout(location = 14) uniform vec3 cameraPostion;

layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;

out vec2 TexCoord;

out vec3 FragPos; 
out vec3 Normal;
out vec3 camera;

void main() {
    gl_Position =  projection * view * model  * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(model))) * normal;  
	TexCoord = vec2((textures.x), (textures.y));
	camera = cameraPostion;
}
