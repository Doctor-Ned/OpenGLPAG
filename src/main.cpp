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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 430 core

    layout (location = 0) in vec3 inPosition;
    layout (location = 1) in vec3 inNormal;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out vec3 exPosition;
	out vec3 exNormal;

    void main()
    {
		exPosition = vec3(model * vec4(inPosition, 1.0));
		exNormal = vec3(model * vec4(inNormal, 1.0));
        gl_Position = projection * view * vec4(exPosition, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 430 core

	in vec3 exPosition;
	in vec3 exNormal;

	uniform vec3 lightPosition;
	uniform vec3 lightColor;
    uniform vec4 color;

    out vec4 outColor;
    void main()
    {
		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * lightColor;
		
		vec3 norm = normalize(exNormal);
		vec3 lightDirection = normalize(lightPosition - exPosition);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = diff * lightColor;
		
		outColor=vec4(ambient+diffuse,1.0f) * color;
    }
)glsl";

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void createCube(glm::vec3 min, glm::vec3 max, float target[], unsigned int offset = 0) {
	float vertices[] = {
		//front
		min.x,min.y,max.z,0.0f,0.0f,1.0f,
		max.x,min.y,max.z,0.0f,0.0f,1.0f,
		max.x,max.y,max.z,0.0f,0.0f,1.0f,
		max.x,max.y,max.z,0.0f,0.0f,1.0f,
		min.x,max.y,max.z,0.0f,0.0f,1.0f,
		min.x,min.y,max.z,0.0f,0.0f,1.0f,
		//back
		min.x,min.y,min.z,0.0f,0.0f,-1.0f,
		min.x,max.y,min.z,0.0f,0.0f,-1.0f,
		max.x,max.y,min.z,0.0f,0.0f,-1.0f,
		max.x,max.y,min.z,0.0f,0.0f,-1.0f,
		max.x,min.y,min.z,0.0f,0.0f,-1.0f,
		min.x,min.y,min.z,0.0f,0.0f,-1.0f,
		//left
		max.x,min.y,max.z,1.0f,0.0f,0.0f,
		max.x,min.y,min.z,1.0f,0.0f,0.0f,
		max.x,max.y,min.z,1.0f,0.0f,0.0f,
		max.x,max.y,min.z,1.0f,0.0f,0.0f,
		max.x,max.y,max.z,1.0f,0.0f,0.0f,
		max.x,min.y,max.z,1.0f,0.0f,0.0f,
		//right
		min.x,min.y,max.z,-1.0f,0.0f,0.0f,
		min.x,max.y,max.z,-1.0f,0.0f,0.0f,
		min.x,max.y,min.z,-1.0f,0.0f,0.0f,
		min.x,max.y,min.z,-1.0f,0.0f,0.0f,
		min.x,min.y,min.z,-1.0f,0.0f,0.0f,
		min.x,min.y,max.z,-1.0f,0.0f,0.0f,
		//top
		min.x,max.y,max.z,0.0f,1.0f,0.0f,
		max.x,max.y,max.z,0.0f,1.0f,0.0f,
		max.x,max.y,min.z,0.0f,1.0f,0.0f,
		max.x,max.y,min.z,0.0f,1.0f,0.0f,
		min.x,max.y,min.z,0.0f,1.0f,0.0f,
		min.x,max.y,max.z,0.0f,1.0f,0.0f,
		//bottom
		min.x,min.y,max.z,0.0f,-1.0f,0.0f,
		min.x,min.y,min.z,0.0f,-1.0f,0.0f,
		max.x,min.y,min.z,0.0f,-1.0f,0.0f,
		max.x,min.y,min.z,0.0f,-1.0f,0.0f,
		max.x,min.y,max.z,0.0f,-1.0f,0.0f,
		min.x,min.y,max.z,0.0f,-1.0f,0.0f
	};
	memcpy(&target[offset], vertices, sizeof(vertices));
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

	glEnable(GL_DEPTH_TEST);   // this is so important. Spent 2 hours looking for it

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

	GLint modelLocation = glGetUniformLocation(shaderProgram, "model"),
		viewLocation = glGetUniformLocation(shaderProgram, "view"),
		projectionLocation = glGetUniformLocation(shaderProgram, "projection"),
		lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPosition"),
		lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor"),
		colorLocation = glGetUniformLocation(shaderProgram, "color");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	float halfSqrt2 = sqrt(2) / 4.0f;

	createCube(glm::vec3(-halfSqrt2, -halfSqrt2, -halfSqrt2), glm::vec3(halfSqrt2, halfSqrt2, halfSqrt2), vertices);

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glm::vec4 color(1.00f, 0.08f, 0.40f, 1.00f), lightColor(1.00f, 1.00f, 1.00f, 1.00f), clearColor(0.352f, 0.392f, 0.92f, 1.00f);
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	glm::vec3 lightPosition(0.0f, 1.0f, 2.0f);

	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SMALLER_SIZE / (float)SMALLER_SIZE, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 2.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static int carpetDepth = 0;
		static int targetDepth = carpetDepth;
		static int carpetSize = SMALLER_SIZE;
		static int targetSize = carpetSize;
		static bool refill = false;
		static bool outline = false;
		static glm::vec2 rotation(0.0f, 0.0f), prevRotation(1.0f, 0.0f);

		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit4("Color", (float*)&color);
			ImGui::SliderInt("Size", &targetSize, 50, SMALLER_SIZE);
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
			ImGui::SliderAngle("X rotation", &(rotation.x));
			ImGui::SliderAngle("Y rotation", &(rotation.y));
			ImGui::NewLine();
			ImGui::ColorEdit3("Light color", (float*)&lightColor);
			ImGui::NewLine();
			ImGui::Checkbox("Outline", &outline);
			ImGui::ColorEdit3("Clear color", (float*)&clearColor);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (glm::any(glm::notEqual(rotation, prevRotation))) {
			prevRotation.x = rotation.x;
			prevRotation.y = rotation.y;
			model = glm::mat4(1.0f);
			model = glm::rotate(model, rotation.x, xAxis);
			model = glm::rotate(model, rotation.y, yAxis);
		}

		if (refill) {
			refill = false;
			//currentSize = refillVao(carpetDepth, &vbo, &vao);
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (outline)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glViewport((SMALLER_SIZE - carpetSize) / 2, (SMALLER_SIZE - carpetSize) / 2, carpetSize, carpetSize);
		glUseProgram(shaderProgram);

		glUniform4f(colorLocation, color.x, color.y, color.z, color.w);
		glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(cubeVAO);
		glBindVertexBuffer(0, VBO, 0, 6 * sizeof(float));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		if (outline)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &cubeVAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}