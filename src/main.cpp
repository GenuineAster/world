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
#include <cstring>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <exception>
#include <functional>

#include "Camera.hpp"
#include "Tile.hpp"
#include "TileMap.hpp"
#include "RelTileMap.hpp"
#include "DrawableGrid.hpp"
#include "Util/FrameTimer.hpp"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <FreeImagePlus.h>

Camera camera;
struct {
	glm::dvec2 prev;
	glm::dvec2 current;
	glm::vec2 delta;
} mouse;

constexpr struct {float x,y;} resolution {1280, 720};

void initCallbacks(GLFWwindow *window);

constexpr float ui_size = 0.3;

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

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		return -1;
	}


	ImGui_ImplGlfwGL3_Init(window, true);
	// ImGui configuration block
	{
		auto &io = ImGui::GetIO();
		io.LogFilename = nullptr;
		io.IniFilename = nullptr;
	}

	initCallbacks(window);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);


	// Shader setup
	Graphics::OpenGL::Shader render_vertex(GL_VERTEX_SHADER);
	render_vertex.setSourceFromFile("res/shaders/shader.vert");
	render_vertex.compile();

	Graphics::OpenGL::Shader render_geometry(GL_GEOMETRY_SHADER);
	render_geometry.setSourceFromFile("res/shaders/shader.geom");
	render_geometry.compile();

	Graphics::OpenGL::Shader render_fragment(GL_FRAGMENT_SHADER);
	render_fragment.setSourceFromFile("res/shaders/shader.frag");
	render_fragment.compile();

	Graphics::OpenGL::ShaderProgram render_shader;
	render_shader.create();
	render_shader.attach(render_vertex);
	render_shader.attach(render_geometry);
	render_shader.attach(render_fragment);
	render_shader.link();
	render_shader.bindFragDataLocation("fColor", 0);
	render_shader.use();

	Graphics::OpenGL::Framebuffer render_fb;
	render_fb.create();
	render_fb.bind();
	Graphics::OpenGL::Texture render_color;
	render_color.create();
	render_color.bind(GL_TEXTURE10, GL_TEXTURE_2D);
	render_color.texImage2D(0, GL_RGB8, resolution.x - resolution.x * ui_size, resolution.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	render_fb.bindTexture2D(GL_COLOR_ATTACHMENT0, render_color, 0);
	Graphics::OpenGL::Texture render_depth;
	render_depth.create();
	render_depth.bind(GL_TEXTURE11, GL_TEXTURE_2D);
	render_depth.texImage2D(0, GL_DEPTH_COMPONENT32, resolution.x - resolution.x * ui_size, resolution.y, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	render_fb.bindTexture2D(GL_DEPTH_ATTACHMENT, render_depth, 0);
	render_fb.drawBuffers({GL_COLOR_ATTACHMENT0});

	// Uniform setup/defaults
	float near = 0.1f/256.f;
	auto near_uniform = render_shader.getUniformLocation("uNear");
	render_shader.setUniformData(near_uniform, near);

	float far = 1000000.f/256.f;
	auto far_uniform = render_shader.getUniformLocation("uFar");
	render_shader.setUniformData(far_uniform, far);

	glm::mat4 projection = glm::perspective(glm::radians(90.f), resolution.x/resolution.y, near, far);
	auto projection_uniform = render_shader.getUniformLocation("uProjection");
	render_shader.setUniformData(projection_uniform, projection);

	glm::mat4 model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
	auto model_uniform = render_shader.getUniformLocation("uModel");
	render_shader.setUniformData(model_uniform, model);

	camera.position = {0.f, 0.f, 1.f};
	glm::mat4 view = camera.getTransform();
	auto view_uniform = render_shader.getUniformLocation("uView");
	render_shader.setUniformData(view_uniform, view);



	Graphics::OpenGL::Framebuffer grid_fb;
	grid_fb.create();
	grid_fb.bind();
	Graphics::OpenGL::Texture grid_color;
	grid_color.create();
	grid_color.bind(GL_TEXTURE13, GL_TEXTURE_2D);
	grid_color.texImage2D(0, GL_RGB8, resolution.x - resolution.x * ui_size, resolution.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	grid_fb.bindTexture2D(GL_COLOR_ATTACHMENT0, grid_color, 0);
	// Graphics::OpenGL::Texture grid_depth;
	// grid_depth.create();
	// grid_depth.bind(GL_TEXTURE14, GL_TEXTURE_2D);
	// grid_depth.texImage2D(0, GL_DEPTH_COMPONENT32, resolution.x - resolution.x * ui_size, resolution.y, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// grid_fb.bindTexture2D(GL_DEPTH_ATTACHMENT, grid_depth, 0);
	// grid_fb.drawBuffers({GL_COLOR_ATTACHMENT0});

	Graphics::OpenGL::Shader grid_vertex(GL_VERTEX_SHADER);
	grid_vertex.setSourceFromFile("res/shaders/grid.vert");
	grid_vertex.compile();

	Graphics::OpenGL::Shader grid_fragment(GL_FRAGMENT_SHADER);
	grid_fragment.setSourceFromFile("res/shaders/grid.frag");
	grid_fragment.compile();

	Graphics::OpenGL::ShaderProgram grid_shader;
	grid_shader.create();
	grid_shader.attach(grid_vertex);
	grid_shader.attach(grid_fragment);
	grid_shader.link();
	grid_shader.bindFragDataLocation("fColor", 0);
	grid_shader.use();

	grid_shader.setUniformData(grid_shader.getUniformLocation("uColor"), render_color);
	grid_shader.setUniformData(grid_shader.getUniformLocation("uDepth"), render_depth);
	grid_shader.setUniformData(grid_shader.getUniformLocation("uProj"), projection);
	auto grid_view = grid_shader.getUniformLocation("uView");


	Graphics::OpenGL::Shader display_vertex(GL_VERTEX_SHADER);
	display_vertex.setSourceFromFile("res/shaders/display.vert");
	display_vertex.compile();

	Graphics::OpenGL::Shader display_fragment(GL_FRAGMENT_SHADER);
	display_fragment.setSourceFromFile("res/shaders/display.frag");
	display_fragment.compile();

	Graphics::OpenGL::ShaderProgram display_shader;
	display_shader.create();
	display_shader.attach(display_vertex);
	display_shader.attach(display_fragment);
	display_shader.link();
	display_shader.bindFragDataLocation("fColor", 0);
	display_shader.use();

	display_shader.setUniformData(display_shader.getUniformLocation("uColor"), grid_color);

	DrawableGrid<uint32_t, GL_UNSIGNED_INT> grid(128);

	TileMap map;
	std::string map_file = "res/maps/basic/basic.json";
	map.loadFromFile(map_file.c_str());
	
	RelTileMap rel_map(5);

	Graphics::OpenGL::Texture dirt;
	{
		fipImage dirt_img;
		dirt_img.load("res/dirt.png");
		dirt.create();
		dirt.bind(GL_TEXTURE3, GL_TEXTURE_2D);
		dirt.texImage2D(0, GL_RGB, dirt_img.getWidth(), dirt_img.getHeight(), GL_BGR, GL_UNSIGNED_BYTE, dirt_img.accessPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	render_shader.use();
	render_shader.setUniformData(render_shader.getUniformLocation("uTex"), dirt);

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

			if (!ImGui::GetIO().WantCaptureKeyboard) {
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
					camera.position += camera.getDirection() * (frametime * 1.f);
				}
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
					camera.position -= camera.getRight() * (frametime * 1.f);
				}
				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
					camera.position -= camera.getDirection() * (frametime * 1.f);
				}
				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
					camera.position += camera.getRight() * (frametime * 1.f);
				}
				if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
					camera.position += camera.getUp() * (frametime * 1.f);
				}
				if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
					camera.position -= camera.getUp() * (frametime * 1.f);
				}
			}
			
			render_shader.use();
			render_fb.bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

			view = camera.getModulatedTransform({1.f, 1.f});
			render_shader.setUniformData(view_uniform, view);

			glViewport(0, 0, resolution.x - resolution.x * ui_size, resolution.y);


			for (const auto &base_index : rel_map.getIndices()) {
				const auto index = base_index + camera.getPositionTileIndex({1.f, 1.f});

				auto &tile = map.getTileOrDefault(index.x, index.y);
				tile.getHeightmap().bind(GL_TEXTURE1, GL_TEXTURE_2D);
				render_shader.setUniformData(render_shader.getUniformLocation("uHeightmap"), tile.getHeightmap());
				model = glm::translate(glm::mat4{1.f}, glm::vec3{glm::vec2(base_index), 0.f});
				render_shader.setUniformData(model_uniform, model);
				grid.draw();
			}


			grid_shader.use();
			grid_shader.setUniformData(grid_view, view);
			grid_fb.bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			display_shader.use();
			Graphics::OpenGL::Framebuffer::bind(nullptr);

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			
			glViewport(0, 0, resolution.x, resolution.y);

			{
				auto &io = ImGui::GetIO();
				ImGui_ImplGlfwGL3_NewFrame(frametime);
				static bool imwin = true;
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.25);
				ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.2, 0.1, 0.1, 1.0});
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0);
				ImGui::SetNextWindowPos({io.DisplaySize.x - ui_size*io.DisplaySize.x, 0.f});
				ImGui::SetNextWindowSize({ui_size*io.DisplaySize.x, io.DisplaySize.y});
				ImGui::Begin("window", &imwin,
					ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse | 0
				);
				{
					char path[512];
					std::strcpy(path, map_file.data());
					ImGui::Text("Heightmap file");
					ImGui::SameLine();
					ImGui::InputText("", path, 512);
					if (path != map_file) {
						try {
							map.loadFromFile(path);
							map_file = path;
						} catch (const std::exception &e) {}
					}
				}
				ImGui::End();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
			}
			ImGui::Render();

			glfwSwapBuffers(window);
		}

		frame_timer.end = std::chrono::high_resolution_clock::now();

		frame_timer.sum += frame_timer.getSeconds();
		frame_timer.count++;

		frame_timer.sleepForFPS(100);

		if (glfwGetTime() > 2.0) {
			double avg = frame_timer.sum / frame_timer.count;
			std::cout<<"avg frametime: "<<std::setw(12)<<static_cast<std::size_t>(1e9*avg)<<"ns"<<std::endl;

			std::cout<<"position:"<<camera.position<<std::endl;

			frame_timer = Util::FrameTimer();
			glfwSetTime(0.0);
		}
	}

	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

void initCallbacks(GLFWwindow *window) {
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			switch (key) {
				case GLFW_KEY_ESCAPE: {
					glfwSetWindowShouldClose(window, true);
				} break;
				
				default: break;
			}
		}

		ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			glfwGetCursorPos(window, &mouse.prev.x, &mouse.prev.y);
		}

		ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
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
}
