#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/io.hpp>

#include <GLFW/glfw3.h>

#include "OpenGL/Shader.hpp"
#include "OpenGL/ShaderProgram.hpp"
#include "OpenGL/Framebuffer.hpp"

#include <array>
#include <thread>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <exception>
#include <functional>

#include <json/json.h>

#include "Camera.hpp"
#include "Tile.hpp"
#include "DrawableGrid.hpp"
#include "Util/FrameTimer.hpp"

Camera camera;
struct {
	glm::dvec2 prev;
	glm::dvec2 current;
	glm::vec2 delta;
} mouse;

constexpr struct {float x,y;} resolution {1280, 720};

int main() {
	// Window/context setup
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(resolution.x, resolution.y, "world", NULL, NULL);
	if (!window) {
		return -1;
	}

	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int, int action, int) {
		if (action == GLFW_PRESS) {
			switch (key) {
				case GLFW_KEY_ESCAPE: {
					glfwSetWindowShouldClose(window, GL_TRUE);
				} break;
				
				default: break;
			}
		}
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int, int) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			glfwGetCursorPos(window, &mouse.prev.x, &mouse.prev.y);
		}
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
			mouse.current = {x, y};
			mouse.delta = mouse.current - mouse.prev;
			mouse.prev = mouse.current;

			glm::vec3 up   { 0.f, 0.f,-1.f};
			glm::vec3 right{ 1.f, 0.f, 0.f};

			glm::quat xrot = glm::angleAxis(-mouse.delta.x/300.f, glm::inverse(camera.rotation) * up);
			glm::quat yrot = glm::angleAxis(-mouse.delta.y/200.f, right);
			camera.rotation *=  xrot * yrot;
			camera.rotation = glm::normalize(camera.rotation);
		}
	});

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);


	// Shader setup
	Graphics::OpenGL::Shader vertex(GL_VERTEX_SHADER);
	vertex.setSourceFromFile("res/shaders/shader.vert");
	vertex.compile();

	Graphics::OpenGL::Shader fragment(GL_FRAGMENT_SHADER);
	fragment.setSourceFromFile("res/shaders/shader.frag");
	fragment.compile();

	Graphics::OpenGL::ShaderProgram shader;
	shader.create();
	shader.attach(vertex);
	shader.attach(fragment);
	shader.link();
	shader.bindFragDataLocation("fColor", 0);
	shader.use();

	// Uniform setup/defaults
	float near = 0.1f;
	auto near_uniform = shader.getUniformLocation("uNear");
	shader.setUniformData(near_uniform, near);

	float far = 1000000.f;
	auto far_uniform = shader.getUniformLocation("uFar");
	shader.setUniformData(far_uniform, far);

	glm::mat4 projection = glm::perspective(glm::radians(90.f), resolution.x/resolution.y, near, far);
	auto projection_uniform = shader.getUniformLocation("uProjection");
	shader.setUniformData(projection_uniform, projection);

	glm::mat4 model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
	auto model_uniform = shader.getUniformLocation("uModel");
	shader.setUniformData(model_uniform, model);


	camera.target = {0.f, 0.f, 1.f};
	glm::mat4 view = camera.getTransform();
	auto view_uniform = shader.getUniformLocation("uView");
	shader.setUniformData(view_uniform, view);


	DrawableGrid<> grid(256);
	
	Tile tile(&grid);
	tile.loadFromFile("res/test.png");

	Util::FrameTimer frame_timer;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	auto frame_start = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window)) {
		// Get frametime
		float frametime = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now() - frame_start
		).count() / 1e6;
		frame_start = std::chrono::high_resolution_clock::now();


		// Only time the actual frame code
		frame_timer.start = std::chrono::high_resolution_clock::now();

		{
			glfwPollEvents();

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				camera.target += camera.getDirection() * (frametime * 1.f);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				camera.target -= camera.getRight() * (frametime * 1.f);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				camera.target -= camera.getDirection() * (frametime * 1.f);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				camera.target += camera.getRight() * (frametime * 1.f);
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				camera.target += camera.getUp() * (frametime * 1.f);
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				camera.target -= camera.getUp() * (frametime * 1.f);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			view = camera.getTransform();
			shader.setUniformData(view_uniform, view);

			shader.setUniformData(shader.getUniformLocation("uHeightmap"), tile.heightmap);
			tile.draw();

			glfwSwapBuffers(window);
		}

		frame_timer.end = std::chrono::high_resolution_clock::now();

		frame_timer.sum += frame_timer.getSeconds();
		frame_timer.count++;

		frame_timer.sleepForFPS(100);

		if (glfwGetTime() > 2.0) {
			double avg = frame_timer.sum / frame_timer.count;
			std::cout<<"avg frametime: "<<std::setw(12)<<static_cast<std::size_t>(1e9*avg)<<"ns"<<std::endl;

			std::cout<<"position:"<<camera.target<<std::endl;

			frame_timer = Util::FrameTimer();
			glfwSetTime(0.0);
		}
	}
}
