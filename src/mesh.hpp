#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>

namespace duck {

struct Mesh {
    GLuint vao;
	GLuint vaoNormals;
    GLuint vertexBuffer;
	GLuint normalsBuffer;
	GLuint textureBuffer;
    GLuint indexBuffer;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
    std::vector<unsigned int> indices;
    //TODO edge information
	bool edgesPresent;
	std::vector<glm::vec3> edgePositions;
	std::vector<unsigned int> edgeTriangles;
	std::vector<bool> triangleFrontFacing;
    //TODO optional texture coordinates

    static Mesh loadQuad(const char* filename, bool edges = false);
	static Mesh loadDuck(const char* filename, bool edges = false);
};

}
