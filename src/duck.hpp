#pragma once

#include "mesh.hpp"
#include <cstdlib>
#include <random>
#include <math.h>
#include <ctime>
#include "constants.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <fstream>
#define M_PI 3.1415926535897932384626433832795
#include <algorithm> 

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

using namespace glm;
using namespace std;

namespace duck {

	class Duck {
	public:
		void run();

	private:
		void init();
		void loop();
		void cleanup();

		void handleEvents();
		void update();
		void updateCamera();
		void render();
		void renderObjects(glm::mat4 view);
		vec2 getBezier();
		unsigned int loadCubemap();
		float _time;
		float h = 2.0f / (256.0f - 1.0f);
		float c = 1.0f;
		float deltaT = 1.0f / 256.0f;
		int N = 256;
		unsigned int loadCubemapPerlin();
		float* squareArray = new float[65536];
		float* squareArrayOld = new float[65536];
		//float squareArrayOld[196608];
		void createSquareArray();
		void updateSquareArray(float x, float y);
		vector<char*> faces
		{
			"resources/negx.jpg",
			"resources/posx.jpg",//back
			"resources/posy.jpg",//right
			"resources/negy.jpg",//left
			"resources/negz.jpg",//front
			"resources/posz.jpg", //ok,
			"resources/ducktex.jpg" //duck
		};
		vector<char*> perlins
		{
			"resources/perlin.jpg",
			"resources/perlin.jpg",//back
			"resources/perlin.jpg",//right
			"resources/perlin.jpg",//left
			"resources/perlin.jpg",//front
			"resources/perlin.jpg", //ok,
			"resources/perlin.jpg" //duck
		};
		unsigned int cubemapTexture;
		GLuint createShaderFromFile(const char* filename, GLenum shaderType);
		int width[7], height[7], nrChannels[7];
		unsigned char *data1;
		unsigned char *data2;
		unsigned char *data3;
		unsigned char *data4;
		unsigned char *data5;
		unsigned char *data6;
		unsigned char *duck;

		SDL_Window* window;
		SDL_GLContext context;
		unsigned int textureID;
		float angleRot = 0.0f;
		vec3 trans;

		glm::mat4 viewMatrix;
		glm::mat4 waterMatrix;
		glm::mat4 waterMatrix2;
		glm::mat4 projectiomMatrix;

		Mesh duckMesh;
		glm::mat4 duckMatrix;
		Mesh quadMesh;
		Mesh waterMesh;
		Mesh waterMesh2;
		GLuint duckProgram;
		GLuint phongProgram;
		GLuint waterProgram;

		bool running;
		bool simulating;
		bool fullscreen;

		Uint32 lastTicks;
		float dt;
		float dtPrim;
		float place = 0.0f;
		float prevPlace = 0.0f;
		vec2 bezirRoots[4];

		bool movingCamera;
		glm::vec3 cameraPosition;
		glm::vec2 cameraRotationDegrees;
		const float cameraMoveSpeed = 2.f;
		const float cameraRotationSpeed = 0.2f;

		glm::mat4 plateMatrix;
		glm::mat4 plateMatrixPrim;

		glm::mat4 groundMatrix[6];

		glm::vec3 lightPosition;
		std::vector<glm::vec3> shadowVolumeVertices;
		std::vector<unsigned int> shadowVolumeIndices;
		GLuint shadowVolumeVao;
		GLuint shadowVolumeVb;
		GLuint shadowVolumeIb;

	};

}
