#include "duck.hpp"


void duck::Duck::run() {
	init();
	loop();
	cleanup();
}
vec2 duck::Duck::getBezier() {

	prevPlace = place;
	place += dtPrim;
	if (place > 1.0f) {
		float rand1 = (float)(rand() % 8) + 1.0f;
		float rand2 = (float)(rand() % 8) + 1.0f;
		bezirRoots[0] = bezirRoots[1];
		bezirRoots[1] = bezirRoots[2];
		bezirRoots[2] = bezirRoots[3];
		bezirRoots[3].x = rand1;// (float)((int)(bezirRoots[3].x + rand1) % 10);
		bezirRoots[3].y = rand2;// (float)((int)(bezirRoots[3].y + rand2) % 10);
		place = 0.0f;
	}

	float beziers[4];
	beziers[0] = pow(1 - place, 3.0f);
	beziers[1] = 3 * pow(1 - place, 2.0f) * place;
	beziers[2] = 3 * (1 - place) * pow(place, 2.0f);
	beziers[3] = pow(place, 3.0f);
	vec2 smallBezier[4];
	smallBezier[0] = ((bezirRoots[0] + bezirRoots[1]) * 0.66f + 0.33f * (bezirRoots[1] + bezirRoots[2])) * 0.5f;
	smallBezier[1] = (bezirRoots[1] + bezirRoots[2]) * 0.33f;
	smallBezier[2] = 0.66f * (bezirRoots[1] + bezirRoots[2]);
	smallBezier[3] = (0.66f * (bezirRoots[1] + bezirRoots[2]) + 0.33f * (bezirRoots[3] + bezirRoots[2])) * 0.5f;
	vec2 help = vec2(smallBezier[0].x * beziers[0] + smallBezier[1].x * beziers[1] + smallBezier[2].x * beziers[2] + smallBezier[3].x * beziers[3],
		smallBezier[0].y * beziers[0] + smallBezier[1].y * beziers[1] + smallBezier[2].y * beziers[2] + smallBezier[3].y * beziers[3]);
	beziers[0] = pow(1 - prevPlace, 3.0f);
	beziers[1] = 3 * pow(1 - prevPlace, 2.0f) * prevPlace;
	beziers[2] = 3 * (1 - prevPlace) * pow(prevPlace, 2.0f);
	beziers[3] = pow(prevPlace, 3.0f);
	vec2 help2 = vec2(smallBezier[0].x * beziers[0] + smallBezier[1].x * beziers[1] + smallBezier[2].x * beziers[2] + smallBezier[3].x * beziers[3],
		smallBezier[0].y * beziers[0] + smallBezier[1].y * beziers[1] + smallBezier[2].y * beziers[2] + smallBezier[3].y * beziers[3]);
	vec2 help1 = normalize(help - help2);
	if (place != 0.0f)
		angleRot = atan2(help1.x, help1.y) - atan2(1, 0) + M_PI;

	return help;
}

void duck::Duck::handleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: {
			running = false;
		} break;
		case SDL_WINDOWEVENT: {
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				glViewport(0, 0, event.window.data1, event.window.data2);
				projectiomMatrix = glm::perspective(glm::radians(45.f), event.window.data1 / (float)event.window.data2, 0.1f, 100.f);
			}
		} break;
		case SDL_MOUSEMOTION: {
			if (movingCamera) {
				cameraRotationDegrees += glm::vec2(event.motion.yrel, event.motion.xrel) * cameraRotationSpeed;
				cameraRotationDegrees.x = glm::clamp(cameraRotationDegrees.x, -90.f, 90.f);
				cameraRotationDegrees.y = fmod(cameraRotationDegrees.y, 360);
			}
		} break;
		case SDL_MOUSEBUTTONDOWN: {
			if (event.button.button == SDL_BUTTON_LEFT) {
				SDL_SetRelativeMouseMode(SDL_TRUE);
				movingCamera = true;
			}
		} break;
		case SDL_MOUSEBUTTONUP: {
			if (event.button.button == SDL_BUTTON_LEFT) {
				SDL_SetRelativeMouseMode(SDL_FALSE);
				movingCamera = false;
			}
		} break;
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				running = false;
				break;
			case SDLK_SPACE:
				simulating = !simulating;
				break;
			case SDLK_TAB:
				//occludingParticles = !occludingParticles;
				break;
			case SDLK_f:
				if (fullscreen) {
					SDL_SetWindowFullscreen(window, 0);
				}
				else {
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				fullscreen = !fullscreen;

				break;
			}
		}
						  break;
		default:
			break;
		}
	}
}

GLuint duck::Duck::createShaderFromFile(const char* filename, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	std::ifstream f;
	f.open(filename, std::ifstream::in | std::ifstream::binary);

	f.seekg(0, std::ios::end);
	int length = f.tellg();
	f.seekg(0, std::ios::beg);

	char* buffer = new char[length];
	f.read(buffer, length);
	f.close();

	glShaderSource(shader, 1, &buffer, &length);
	glCompileShader(shader);

	int  success;
	char log[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, NULL, log);
		SDL_Log("%s:\n%s", filename, log);
		throw std::runtime_error("Shader compilation failed");
	}

	delete[] buffer;
	return shader;
}


void duck::Duck::updateCamera() {
	viewMatrix = glm::mat4(1);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotationDegrees.x), { 1, 0, 0 });
	viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotationDegrees.y), { 0, 1, 0 });
	viewMatrix = glm::translate(viewMatrix, -cameraPosition);

	glm::vec4 cameraFront = { 0, 0, -1, 0 };
	glm::vec4 cameraUp = { 0, 1, 0, 0 };
	glm::vec4 cameraSide = { 1, 0, 0, 0 };

	cameraFront = cameraFront * viewMatrix;
	cameraUp = cameraUp    * viewMatrix;
	cameraSide = cameraSide  * viewMatrix;

	const Uint8* keyboardState = SDL_GetKeyboardState(0);
	if (keyboardState[SDL_SCANCODE_W])
		cameraPosition += glm::vec3(cameraFront) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_S])
		cameraPosition -= glm::vec3(cameraFront) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_D])
		cameraPosition += glm::vec3(cameraSide) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_A])
		cameraPosition -= glm::vec3(cameraSide) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_LSHIFT])
		cameraPosition += glm::vec3(cameraUp) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_LCTRL])
		cameraPosition -= glm::vec3(cameraUp) * dt * cameraMoveSpeed;
}

void duck::Duck::cleanup() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
