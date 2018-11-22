#include "Shader.h"
#include "UboLight.h"
#include "UboViewProjection.h"
#include "MengerSponge.h"
#include <cstdio>
#include <vector>
#include "MeshCylinder.h"

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

	Shader shader("vertexShader.glsl", "fragmentShader.glsl");


	GLint modelLocation = glGetUniformLocation(shader.getID(), "model"),
		colorLocation = glGetUniformLocation(shader.getID(), "color"),
		disableTextureLocation = glGetUniformLocation(shader.getID(), "disableTexture");

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

	glm::vec4 color(1.00f, 1.00f, 1.00f, 1.00f), lightColor(1.00f, 1.00f, 1.00f, 1.00f), clearColor(0.352f, 0.392f, 0.92f, 1.00f), prevLightColor = lightColor;
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	glm::vec4 lightPosition(0.0f, 1.0f, 2.0f, 1.0f);

	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SMALLER_SIZE / (float)SMALLER_SIZE, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 2.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model(1.0f);

	UboViewProjection uboViewProjection(view, projection);
	UboLight uboLight(lightPosition, lightColor);

	shader.bind(&uboViewProjection);
	shader.bind(&uboLight);

	static float radius = 0.3f;
	static float height = 1.0f;
	static int sideAmount = 3;
	MeshCylinder cylinder(radius, height, sideAmount, "texture_cylinder.jpg");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static float targetRadius = radius;
		static float targetHeight = height;
		static int targetSides = sideAmount;
		static int outputSize = SMALLER_SIZE;
		static int targetSize = outputSize;
		static bool refill = false;
		static bool outline = false;
		static bool shouldUseTexture = true, shouldAutoUpdate = true;
		static glm::vec2 rotation(0.5f, 1.0f), prevRotation(0.0f, 0.0f);


		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit3("Color", (float*)&color);
			ImGui::SliderInt("Size", &targetSize, 50, SMALLER_SIZE);
			if (outputSize != targetSize) {
				outputSize = targetSize;
			}
			ImGui::NewLine();
			ImGui::SliderFloat("Cylinder radius", &targetRadius, 0.05f, 2.0f);
			ImGui::SameLine();
			ImGui::Text("Cylinder's current radius: %f.", radius);
			ImGui::NewLine();
			ImGui::SliderFloat("Cylinder height", &targetHeight, 0.05f, 2.0f);
			ImGui::SameLine();
			ImGui::Text("Cylinder's current height: %f.", height);
			ImGui::NewLine();
			ImGui::SliderInt("Cylinder sides", &targetSides, 3, 50);
			ImGui::SameLine();
			ImGui::Text("The cylinder has %d sides.", sideAmount);
			ImGui::NewLine();
			if (ImGui::Button("Apply cylinder changes") || shouldAutoUpdate) {
				if (sideAmount != targetSides || radius != targetRadius || height != targetHeight) {
					sideAmount = targetSides;
					radius = targetRadius;
					height = targetHeight;
					cylinder.baseCenter.y = (-height / 2.0f);
					refill = true;
				}
			}
			ImGui::Checkbox("Auto-update", &shouldAutoUpdate);
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
			prevRotation = rotation;
			model = glm::mat4(1.0f);
			model = glm::rotate(model, rotation.x, xAxis);
			model = glm::rotate(model, rotation.y, yAxis);
		}

		if (glm::any(glm::notEqual(lightColor, prevLightColor))) {
			prevLightColor = lightColor;
			uboLight.inject(lightPosition, lightColor);
		}

		if (refill) {
			refill = false;
			cylinder.updateValues(radius, height, sideAmount);
			//cube.recreate(recurseDepth);
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
		glUseProgram(shader.getID());

		glUniform1i(disableTextureLocation, shouldUseTexture ? 0 : 1);
		glUniform4f(colorLocation, color.x, color.y, color.z, color.w);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		cylinder.Draw(shader);
		//cube.render();
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

	shader.remove();
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &cubeVAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}