#include "duck.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define RAND_MAX 65536;

unsigned int texture1;
unsigned int texturePerlin;
unsigned int textureDuck;
int prevX = -1;
int prevY = -1;
int countingFrames = 0;
unsigned int textureSquare;
unsigned int duck::Duck::loadCubemap()
{
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i], &width[i], &height[i], &nrChannels[i], 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
		glGenTextures(1, &textureDuck);
		glBindTexture(GL_TEXTURE_2D, textureDuck);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		duck = stbi_load(faces[6], &width[6], &height[6], &nrChannels[6], 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[6], height[6], 0, GL_RGB, GL_UNSIGNED_BYTE, duck);

	return textureID;
}
unsigned int duck::Duck::loadCubemapPerlin()
{
	glGenTextures(1, &texturePerlin);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texturePerlin);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	for (unsigned int i = 0; i < perlins.size(); i++)
	{
		unsigned char *data = stbi_load(perlins[i], &width[i], &height[i], &nrChannels[i], 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	return textureID;
}

void duck::Duck::createSquareArray() {
	for (int i = 0; i < 256 * 256; i++) {
		squareArray[i] = 0.0f;
		squareArrayOld[i] = 0.0f;
	}
	float* ret = new float[3 * 256 * 256];
	for (int i = 0; i < 256 * 256 * 3; i ++) {
		ret[i] = 0.0f;
	}
	glGenTextures(1, &textureSquare);
	glBindTexture(GL_TEXTURE_2D, textureSquare);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//duck = stbi_load(faces[6], &width[6], &height[6], &nrChannels[6], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 256, 256, 0, GL_RGB, GL_FLOAT, ret);
	delete ret;
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[6], height[6], 0, GL_RGB, GL_UNSIGNED_BYTE, duck);
}

void duck::Duck::updateSquareArray(float x, float y) {
	int ind = 0, step = 1;
	if (prevX == -1 && prevY == -1) {
		prevX = (int)(x * 256);
		prevY = (int)(y * 256);
	}
	else {
		int step = 1;
		int _prevX = prevX;// +128;
		int _prevY = prevY;// +128;
		if (x * 256.0f > prevX) {
			prevX += step;
			ind += ((int)(_prevX));
		}
		else {
			prevX -= step;
			ind += ((int)(_prevX));
		}
		if (y * 256.0f > prevY) {
			prevY += step;
			ind += (int)(_prevY * 256);
		}
		else {
			prevY -= step;
			ind += (int)(_prevY * 256);
		}
	}

	float* _square = new float[256 * 256];
	float _h = 2.0f / (256.0f - 1.0f);
	float _c = 1.0f;
	float _deltaT = 1.0f / 256.0f;
	for (int i = 0; i < 256 * 256; i++) {
		_square[i] = 0.0f;
	} 
	//(squareArray)[500] = 0.25f;
	squareArray[ind % (256 * 256)] = 100.0f;
	if (countingFrames > 10) {
		squareArray[rand() * rand() % (256 *256)] = 100.0f;
		countingFrames = 0;
	}
	countingFrames++;
	//(squareArray)[502] = 0.25f;
	float A = _c * _c * _deltaT * _deltaT / (_h * _h);
	float B = 2 - 4 * A;
	for (int i = 1; i < 255; i++) {
		for (int j = 1; j < 255; j++) {
			float l = glm::max((float)i, (float)j);
			l = glm::max(l, (float)(256.0f - (float)j));
			l = glm::max(l, (float)(256.0f - (float)i));
			l /= 128.0f;
			float dij = 0.95f * glm::min(1.0f, l / 0.2f);
			_square[(i * 256) + j] = squareArray[((i + 1) * 256) + j]
				+ squareArray[((i - 1) * 256) + j]
				+ squareArray[(i * 256) + j - 1]
				+ squareArray[(i * 256) + j + 1];
			_square[(i * 256) + j] *= A;
			_square[(i * 256) + j] = _square[(i * 256) + j]
				+ (squareArray[(i * 256) + j] * B)
				- squareArrayOld[(i * 256) + j];
			_square[(i * 256) + j] *= dij;
		}
	}
	for (int i = 0; i < 256 * 256; i++) {
		squareArrayOld[i] = squareArray[i];
		squareArray[i] = _square[i];
	}
	int j = 0;
	float* ret = new float[256 * 256 * 3];
	for (int i = 0; i < 256 * 256 * 3; i+=3) {
		if (j == 0 || j == 256 * 256) {
			ret[1] = 1.0f;
			ret[i + 1] = 1.0f;
			ret[i + 2] = 1.0f;
			j++;
			continue;
		}
		ret[i] = (squareArray[j - 1] + squareArray[j + 1]) / 2;
		ret[i + 1] = 1.0f;
		ret[i + 2] = (squareArray[(int)(j - 256)] + squareArray[(int)(j + 256)]) / 2;
		j++;
	}
	glBindTexture(GL_TEXTURE_2D, textureSquare);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//duck = stbi_load(faces[6], &width[6], &height[6], &nrChannels[6], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 256, 256, 0, GL_RGB, GL_FLOAT, ret);
	delete ret;
}
void duck::Duck::init() {
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		SDL_Log("%s", SDL_GetError());
		throw std::runtime_error("SDL initialization failed");
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	_time = time(NULL) % 60;
	srand(time(NULL));
	//window = SDL_CreateWindow("duck", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	fullscreen = false;
	window = SDL_CreateWindow("duck", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("%s", SDL_GetError());
		throw std::runtime_error("Window creation failed");
	}

	context = SDL_GL_CreateContext(window);
	if (!context) {
		SDL_Log("%s", SDL_GetError());
		throw std::runtime_error("Context creation failed");
	}
	// NOTE potentially needed?
	//glewExperimental = GL_TRUE;
	GLenum glewRet = glewInit();

	glGenVertexArrays(1, &shadowVolumeVao);
	glBindVertexArray(shadowVolumeVao);
	glGenBuffers(1, &shadowVolumeVb);
	glBindBuffer(GL_ARRAY_BUFFER, shadowVolumeVb);
	glGenBuffers(1, &shadowVolumeIb);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowVolumeIb);


	duckProgram = glCreateProgram();
	GLuint vs = createShaderFromFile("resources/duckVS.glsl", GL_VERTEX_SHADER);
	glAttachShader(duckProgram, vs);
	GLuint fs = createShaderFromFile("resources/duckFS.glsl", GL_FRAGMENT_SHADER);
	glAttachShader(duckProgram, fs);
	glLinkProgram(duckProgram);
	int success;
	char log[1024];
	glGetProgramiv(duckProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(duckProgram, 1024, NULL, log);
		SDL_Log("%s", log);
		throw std::runtime_error("Linking failed");
	}
	phongProgram = glCreateProgram();
	GLuint vs1 = createShaderFromFile("resources/phongVS.glsl", GL_VERTEX_SHADER);
	glAttachShader(phongProgram, vs1);
	GLuint fs1 = createShaderFromFile("resources/phongFS.glsl", GL_FRAGMENT_SHADER);
	glAttachShader(phongProgram, fs1);
	glLinkProgram(phongProgram);
	glGetProgramiv(phongProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(phongProgram, 1024, NULL, log);
		SDL_Log("%s", log);
		throw std::runtime_error("Linking failed");
	}
	waterProgram = glCreateProgram();
	GLuint vs2 = createShaderFromFile("resources/waterVS.glsl", GL_VERTEX_SHADER);
	glAttachShader(waterProgram, vs2);
	GLuint fs2 = createShaderFromFile("resources/waterFS.glsl", GL_FRAGMENT_SHADER);
	glAttachShader(waterProgram, fs2);
	glLinkProgram(waterProgram);
	glGetProgramiv(waterProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(waterProgram, 1024, NULL, log);
		SDL_Log("%s", log);
		throw std::runtime_error("Linking failed");
	}
	duckMesh = Mesh::loadDuck("resources/duck.txt", true);
	loadCubemap();
	loadCubemapPerlin();
	createSquareArray();
	quadMesh = Mesh::loadQuad("resources/quad.txt");
	waterMesh = Mesh::loadQuad("resources/water.txt");
	waterMesh2 = Mesh::loadQuad("resources/water1.txt");

	int winW, winH;
	SDL_GL_GetDrawableSize(window, &winW, &winH);
	projectiomMatrix = glm::perspective(glm::radians(45.f), winW / (float)winH, 0.1f, 100.f);

	movingCamera = false;
	cameraPosition = { 0, 4, 15 };
	cameraRotationDegrees = { 0, 0 };

	plateMatrix = glm::mat4(1);
	waterMatrix = glm::scale(glm::mat4(1), { 10.0f, 10.0f, 10.0f });
	waterMatrix2 = glm::scale((glm::rotate(glm::mat4(1), (float)radians(180.0f), glm::vec3{ 0.0f, 0.0f, 1.0f })), { 10.0f, 10.0f, 10.0f });
	plateMatrixPrim = glm::mat4(1);
	plateMatrix = glm::translate(plateMatrix, { -1.5, 0.3, 0 });
	plateMatrixPrim = glm::translate(plateMatrix, { -2, 0.3, 0 });
	plateMatrix = glm::rotate(plateMatrix, -glm::pi<float>() / 3, { 0, 0, 1 });
	plateMatrixPrim = glm::rotate(plateMatrix, -glm::pi<float>() / 3, { 0, 0, 1 });

	groundMatrix[0] = glm::scale(glm::translate(glm::mat4(1), { 0, -1, 0 }), { 10.0f, 10.0f, 10.0f });
	groundMatrix[1] = glm::scale(glm::translate(glm::rotate(glm::mat4(1), (float)radians(270.0f), glm::vec3{ 1.0f, 0.0f, 0.0f }), { 0.0f, -5.0f, 4.0f }), { 10.0f, 10.0f, 10.0f });
	groundMatrix[2] = glm::scale(glm::translate(glm::rotate(glm::mat4(1), (float)radians(90.0f), glm::vec3{ 0.0f, 0.0f, 1.0f }), { 4.0f, -5.0f, 0 }), { 10.0f, 10.0f, 10.0f });
	groundMatrix[3] = glm::scale(glm::translate(glm::rotate(glm::mat4(1), (float)radians(270.0f), glm::vec3{ 0.0f, 0.0f, 1.0f }), { -4.0f, -5.0f, 0 }), { 10.0f, 10.0f, 10.0f });
	groundMatrix[4] = glm::scale(glm::translate(glm::rotate(glm::mat4(1), (float)radians(90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f }), { 0.0f, -5.0f, -4.0f }), { 10.0f, 10.0f, 10.0f });
	groundMatrix[5] = glm::scale(glm::translate(glm::rotate(glm::mat4(1), (float)radians(180.0f), glm::vec3{ 0.0f, 0.0f, 1.0f }), { 0, -9.0f, 0 }), { 10.0f, 10.0f, 10.0f });

	duckMatrix = glm::scale(glm::mat4(1), { 0.1f, 0.1f, 0.1f });

	lightPosition = glm::vec3(-4.0f, 4.0f, 4.0f);

}


void duck::Duck::loop() {
	running = true;
	simulating = true;
	lastTicks = SDL_GetTicks();
	bezirRoots[0].x = (float)(rand() % 10);
	bezirRoots[1].x = (float)(rand() % 10);
	bezirRoots[2].x = (float)(rand() % 10);
	bezirRoots[3].x = (float)(rand() % 10);
	bezirRoots[0].y = (float)(rand() % 10);
	bezirRoots[1].y = (float)(rand() % 10);
	bezirRoots[2].y = (float)(rand() % 10);
	bezirRoots[3].y = (float)(rand() % 10);
	while (running) {
		Uint32 newTicks = SDL_GetTicks();
		dt = (newTicks - lastTicks) / 1000.f;
		dtPrim = (newTicks - lastTicks) / 3000.f;
		lastTicks = newTicks;
		int first = ((rand()) % 10) - 5.0f;
		int second = ((rand()) % 10) - 5.0f;
		int third = ((rand()) % 10) - 5.0f;
		handleEvents();
		update();
		render();
	}
}
void duck::Duck::update() {
	updateCamera();
	_time = time(NULL) % 60;

	if (!simulating) return;
	vec2 _place = getBezier();

	duckMatrix = glm::translate(mat4(1), { _place.x - 5.0f, 0, _place.y - 5.0f }) * rotate(glm::scale(glm::mat4(1), { 0.5f, 0.5f, 0.5f }), angleRot, { 0, 1,  0 });
	updateSquareArray(_place.x / 10.f, _place.y / 10.f);
	//TODO ik
	// First part 
}

void duck::Duck::render() {
	// prepare data
	// render
	glClearColor(0.0f / 255.0f, 24.0f / 255.0f, 72.0f / 255.0f, 1);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	renderObjects(viewMatrix);


	SDL_GL_SwapWindow(window);
	return;
}

void duck::Duck::renderObjects(glm::mat4 view)
{
	//glUseProgram(phongProgram);
	glDepthMask(GL_TRUE);
	Mesh mesh;
	glUseProgram(duckProgram);
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_VIEW, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_PROJECTION, 1, GL_FALSE, glm::value_ptr(projectiomMatrix));

	//duck
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDuck);
	mesh = duckMesh;
	glBindVertexArray(mesh.vao);
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(duckMatrix));
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

	mesh = quadMesh;
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glBindVertexArray(mesh.vao);
	glUseProgram(phongProgram);
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_VIEW, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_PROJECTION, 1, GL_FALSE, glm::value_ptr(projectiomMatrix));

	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glUniform1i(glGetUniformLocation(phongProgram, "skybox"), 0);
	for (int i = 0; i < 6; i++) {
		glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(groundMatrix[i]));
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	}

	//water
	mesh = waterMesh;
	glUseProgram(waterProgram);
	glUniform1i(glGetUniformLocation(waterProgram, "skybox"), 0);
	glUniform1i(glGetUniformLocation(waterProgram, "perlin"), 1);
	glUniform1i(glGetUniformLocation(waterProgram, "square"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texturePerlin);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureSquare);

	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_VIEW, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(SHADER_UNIFORM_CAMERA_POSITION, 1, glm::value_ptr(cameraPosition));
	glUniform1f(SHADER_UNIFORM_TIME, _time);
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_PROJECTION, 1, GL_FALSE, glm::value_ptr(projectiomMatrix));
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	mesh = waterMesh2;
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(waterMatrix));
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(waterMatrix2));
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}
