#version 330 core

in vec3 Normal;
in vec3 FragPos;  
in vec3 Cam;
out vec4 outColor;
in vec3 TexCoords;
in vec3 TexCoords2;
in vec3 ab_pos;
uniform samplerCube skybox;
uniform samplerCube perlin;
uniform sampler2D square;

vec3 intersectRay(vec3 p, vec3 r) {
	float tx = (sign(r.x) - p.x) / r.x;
	float ty = (sign(r.y) - p.y) / r.y;
	float tz = (sign(r.z) - p.z) / r.z;
	float t = min(min(tx, ty), tz);
	return vec3(p.x + (t * r.x), p.y + (t * r.y), p.z + (t * r.z));
}
float fresnel(vec3 r, vec3 n, float n1, float n2) {	float F0 = pow((n2 - n1) / (n2 + n1), 2.0f);	float cosTeta = max(dot(r, n), 0.0f);	return F0 + (1 - F0) * pow((1 - cosTeta), 5.0f);}
void main() {
	float n1;
	float n2;
	n1 = 1.0f;
	//vec3 norm = normalize(vec3(ex, 20.f, ez)); //float3(1.0f, 1.0f, 0.0f); //normalize(float3(ex, 20.f, ez));
	vec3 sq = texture(square, vec2(ab_pos.x, ab_pos.z)).rgb;
	vec3 norm = normalize(vec3(sq.r, Normal.y, sq.b));
	n2 =  4.0f / 3.0f;
	vec3 viewVec =
		normalize(FragPos - Cam);
	vec3 reflectVector = reflect(viewVec, norm);
	vec3 refractVector = refract(viewVec, norm, Normal.y > 0.0f ? n1 / n2 : n2/n1);
	vec3 col1 = intersectRay(TexCoords2, reflectVector);
	vec3 col2 = intersectRay(TexCoords2, refractVector);
	vec3 i1 = vec4(texture(skybox, col1)).rgb;
	vec3 i2 = vec4(texture(skybox, col2)).rgb;
	//float f = 0.14f;
	float f = 0.0f;
	if (Normal.y > 0.0f) {
		f = fresnel(reflectVector, norm, n1, n2);
	}
	else {
		f = fresnel(reflectVector, norm, n1, n2);
	}
	/*if (!any(refractVector)) {
		outColor = vec4((1 - f) * i1, f);
	}
	else {*/
	if (refractVector.x == 0.0f && refractVector.y == 0.0f && refractVector.z == 0.0f) {
		outColor = vec4(refractVector, 1);
	}
	else {
		outColor = vec4((1 - f) * i1 + f*i2, f);
	}
	//outColor = vec4(refractVector, 1);
	//outColor = vec4((1 - f) * i1 + f*i2, f);
	//}
	//outColor = vec4(col1, 1.0f);
}
