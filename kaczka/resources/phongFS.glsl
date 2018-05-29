#version 330 core

out vec4 outColor;

in vec3 TexCoord;

uniform samplerCube skybox;


void main() {
	outColor = texture(skybox, TexCoord);// * vec4(result, 0.1f);
	//outColor = vec4(TexCoord, 1.0f);
}
