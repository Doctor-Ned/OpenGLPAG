#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include <vector>

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
    in vec3 inPosition;
    uniform vec4 inColor;
    out vec4 exColor;
    void main()
    {
        exColor = inColor;
        gl_Position = vec4(inPosition, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 430 core
    in vec4 exColor;
    out vec4 outColor;
    void main()
    {
        outColor = exColor;
    }
)glsl";

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void createRectangleVertices(ImVec4 *rect, GLfloat *vert, int offset) {
	GLfloat vertices[] = {
		rect->y,rect->w,0.0f,
		rect->y,rect->z,0.0f,
		rect->x,rect->w,0.0f,
		rect->y,rect->z,0.0f,
		rect->x,rect->z,0.0f,
		rect->x,rect->w,0.0f
	};
	memcpy(&(vert[offset]), vertices, sizeof(vertices));
}

ImVec4 *remapRectToColRow(ImVec4 *rect, int col, int row) {
	ImVec4 *result = new ImVec4();
	float width = (rect->y - rect->x) / 3.0f, height = (rect->w - rect->z) / 3.0f;
	result->x = rect->x + width * col;
	result->y = result->x + width;
	result->z = rect->z + height * row;
	result->w = result->z + height;
	return result;
}

ImVec4 *remapRectToColRow(ImVec4 *rect, int index) {
	return remapRectToColRow(rect, index % 3, index / 3);
}

void createCarpetVertices(int depth, ImVec4 *rect, GLfloat *vertices, int off = 0) {
	static int offset = 0;
	if (off == 1) offset = 0;
	if (depth == 0) {
		createRectangleVertices(rect, vertices, offset);
		offset += 6 * 3;
	} else {
		depth--;
		for (int i = 0; i < 9; i++) {
			if (i != 4) {
				ImVec4 *newRect = remapRectToColRow(rect, i);
				createCarpetVertices(depth, newRect, vertices);
				delete newRect;
			}
		}
	}
}

GLsizei refillVao(int recurseDepth, GLuint *vbo, GLuint *vao) {
	static ImVec4 fullRect(-1, 1, -1, 1);  //minX,maxX,minY,maxX
	glBindVertexArray(*vao);
	GLsizei newSize = recurseDepth == 0 ? 1 : pow(8, recurseDepth);
	if (vbo != 0)glDeleteBuffers(1, vbo);
	glGenBuffers(1, vbo);
	GLfloat *vertices = new float[newSize * 6 * 3];
	if (recurseDepth >= 0) {
		createCarpetVertices(recurseDepth, &fullRect, vertices, 1);
		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, newSize * 6 * 3 * sizeof(float), vertices, GL_STATIC_DRAW); // STATIC?
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
	return newSize;
}

int main(int, char**) {
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
	const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 800;
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

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();


	ImVec4 carpetColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f), clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
		fprintf(stderr, "Vertex shader error: %s\n", errorLog);
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
		fprintf(stderr, "Fragment shader error: %s\n", errorLog);
		delete errorLog;
		return 1;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, 0, "inPosition");
	glBindAttribLocation(shaderProgram, 1, "inColor");

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

	GLuint vao = 0, vbo = 0;
	glGenVertexArrays(1, &vao);

	GLsizei currentSize = refillVao(0, &vbo, &vao);

	GLint carpetColorLocation = glGetUniformLocation(shaderProgram, "inColor");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static int carpetDepth = 0;
		static int targetDepth = carpetDepth;
		static int carpetSize = 400;
		static int targetSize = carpetSize;
		static bool refill = false;

		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit3("Carpet color", (float*)&carpetColor);
			ImGui::SliderInt("Carpet size", &targetSize, 50, SMALLER_SIZE);
			if (carpetSize != targetSize) {
				carpetSize = targetSize;
			}
			ImGui::NewLine();
			ImGui::SliderInt("Recursion depth", &targetDepth, 0, 6);
			if (ImGui::Button("Apply recursion depth")) {
				if (carpetDepth != targetDepth) {
					carpetDepth = targetDepth;
					refill = true;
				}
			}
			ImGui::SameLine();
			ImGui::Text("Current depth: %d", carpetDepth);
			ImGui::NewLine();
			ImGui::ColorEdit3("Clear color", (float*)&clearColor);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (refill) {
			refill = false;
			currentSize = refillVao(carpetDepth, &vbo, &vao);
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, carpetSize, carpetSize);
		glUseProgram(shaderProgram);
		glUniform4f(carpetColorLocation, carpetColor.x, carpetColor.y, carpetColor.z, carpetColor.w);
		glBindVertexArray(vao);

		glBindVertexBuffer(0, vbo, 0, 3 * sizeof(float));
		glDrawArrays(GL_TRIANGLES, 0, currentSize * 6);

		glBindVertexArray(0);

		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}