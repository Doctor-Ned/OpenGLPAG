#include "Util.h"
#include "MengerSponge.h"
#include <cstdio>
#include <vector>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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

	GLuint vertexShader = Util::createAndCompileShader(GL_VERTEX_SHADER, "vertexShader.glsl");

	GLuint fragmentShader = Util::createAndCompileShader(GL_FRAGMENT_SHADER, "fragmentShader.glsl");

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	Util::linkShaderProgram(shaderProgram);

	GLint modelLocation = glGetUniformLocation(shaderProgram, "model"),
		viewLocation = glGetUniformLocation(shaderProgram, "view"),
		projectionLocation = glGetUniformLocation(shaderProgram, "projection"),
		lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPosition"),
		lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor"),
		colorLocation = glGetUniformLocation(shaderProgram, "color"),
		disableTextureLocation = glGetUniformLocation(shaderProgram, "disableTexture");

	const char * TEXTURE_NAME = "texture.jpg";

	int imgWidth, imgHeight, imgChannels;
	unsigned char *imgData = stbi_load(TEXTURE_NAME, &imgWidth, &imgHeight, &imgChannels, 0);
	if (!imgData) {
		fprintf(stderr, "Failed to load texture from file \"%s\"!", TEXTURE_NAME);
		return 1;
	}
	GLuint imgTexture;
	glGenTextures(1, &imgTexture);
	glBindTexture(GL_TEXTURE_2D, imgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);

	MengerSponge cube;

	glm::vec4 color(1.00f, 1.00f, 1.00f, 1.00f), lightColor(1.00f, 1.00f, 1.00f, 1.00f), clearColor(0.352f, 0.392f, 0.92f, 1.00f);
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	glm::vec3 lightPosition(0.0f, 1.0f, 2.0f);

	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SMALLER_SIZE / (float)SMALLER_SIZE, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 2.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model(1.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static int recurseDepth = 0;
		static int targetDepth = recurseDepth;
		static int outputSize = SMALLER_SIZE;
		static int targetSize = outputSize;
		static bool refill = false;
		static bool outline = false;
		static bool shouldUseTexture = true;
		static glm::vec2 rotation(0.5f, 1.0f), prevRotation(0.0f, 0.0f);


		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit3("Color", (float*)&color);
			ImGui::SliderInt("Size", &targetSize, 50, SMALLER_SIZE);
			if (outputSize != targetSize) {
				outputSize = targetSize;
			}
			ImGui::NewLine();
			ImGui::SliderInt("Recursion depth", &targetDepth, 0, 4);
			if (ImGui::Button("Apply recursion depth")) {
				if (recurseDepth != targetDepth) {
					recurseDepth = targetDepth;
					refill = true;
				}
			}
			ImGui::SameLine();
			ImGui::Text("Current depth: %d", recurseDepth);
			ImGui::NewLine();
			ImGui::SliderAngle("X rotation", &(rotation.x), -180.0f, 180.0f);
			ImGui::SliderAngle("Y rotation", &(rotation.y), -180.0f, 180.0f);
			ImGui::NewLine();
			ImGui::Checkbox("Use texture", &shouldUseTexture);
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
			cube.recreate(recurseDepth);
		}

		//outputSize = ((sin(glfwGetTime()) + 1.0f)*(SMALLER_SIZE-50.0f) / 2.0f) + 50.0f;

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (outline)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glViewport((SMALLER_SIZE - outputSize) / 2, (SMALLER_SIZE - outputSize) / 2, outputSize, outputSize);
		glUseProgram(shaderProgram);

		glUniform1i(disableTextureLocation, shouldUseTexture ? 0 : 1);
		glUniform4f(colorLocation, color.x, color.y, color.z, color.w);
		glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		cube.render();
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
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &cubeVAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}