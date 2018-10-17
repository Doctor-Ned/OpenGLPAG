#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 430 core
    in vec3 Color;
    out vec4 Color;
    void main()
    {
        Color = vec4(Color, 1.0);
    }
)glsl";

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**) {
	void refillVao(GLuint, int);

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 4.3 + GLSL 430
	const char* glsl_version = "#version 430";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLPAG", NULL, NULL);
	if (window == NULL) {
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
	if (err) {
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();


	ImVec4 carpet_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f), clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	//load shaders

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);
	GLint isFine, maxLength;
	char *errorLog;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isFine);
	if (isFine == FALSE) {
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		errorLog = new char[maxLength];
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, errorLog);
		fprintf(stderr, errorLog);
		delete errorLog;
		return 1;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFine);
	if (isFine == FALSE) {
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		errorLog = new char[maxLength];
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, errorLog);
		fprintf(stderr, errorLog);
		delete errorLog;
		return 1;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, 0, "in_position");
	glBindAttribLocation(shaderProgram, 1, "in_color");

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isFine);
	if (isFine == FALSE) {
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		errorLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, errorLog);
		fprintf(stderr, errorLog);
		delete errorLog;
		return 1;
	}

	GLuint vao;
	//glGenVertexArrays(1, &vao);
	//refillVao(vao, 1);

	glUseProgram(shaderProgram);

	while (!glfwWindowShouldClose(window)) {
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.


		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Configuration window
		static int carpetDepth = 1;
		static int targetDepth = carpetDepth;
		static int carpetSize = 400;
		static int targetSize = carpetSize;
		static bool refill = false;

		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit3("Carpet color", (float*)&carpet_color);
			ImGui::SliderInt("Carpet size", &targetSize, 50, SMALLER_SIZE);
			if (ImGui::Button("Apply carpet size")) {
				if (carpetSize != targetSize) {
					carpetSize = targetSize;
				}
			}
			ImGui::SameLine();
			ImGui::Text("Current size: %d", carpetSize);
			ImGui::NewLine();
			ImGui::SliderInt("Recursion depth", &targetDepth, 1, 20);
			if (ImGui::Button("Apply recursion depth")) {
				if (carpetDepth != targetDepth) {
					carpetDepth = targetDepth;
					refill = true;
				}
			}
			ImGui::SameLine();
			ImGui::Text("Current depth: %d", carpetDepth);
			ImGui::NewLine();
			ImGui::ColorEdit3("Clear color", (float*)&clear_color);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (refill) {
			refill = false;
			refill(carpetDepth, vao);
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void refillVao(int recurseDepth, GLuint vao) {

}