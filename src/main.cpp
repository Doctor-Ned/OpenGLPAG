#include "UboLight.h"
#include "UboViewProjection.h"
#include <cstdio>
#include "MeshCylinder.h"
#include "MeshCone.h"
#include "GraphNode.h"
#include "Model.h"
#include "MeshOrbit.h"

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

	Shader texturedShader("textureVertexShader.glsl", "textureFragmentShader.glsl");
	Shader solidShader("solidVertexShader.glsl", "solidFragmentShader.glsl");
	Shader simpleShader("simpleVertexShader.glsl", "simpleFragmentShader.glsl");


	GLint disableTextureLocation = texturedShader.getUniformLocation("disableTexture");

	glm::vec4 color(1.00f, 1.00f, 1.00f, 1.00f), lightColor(1.00f, 1.00f, 1.00f, 1.00f), clearColor(0.352f, 0.392f, 0.92f, 1.00f), prevLightColor = lightColor;
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	glm::vec4 lightPosition(0.0f, 1.0f, 2.0f, 1.0f);

	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SMALLER_SIZE / (float)SMALLER_SIZE, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 2, 5.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model(1.0f);

	UboViewProjection uboViewProjection(view, projection);
	UboLight uboLight(lightPosition, lightColor);

	texturedShader.bind(&uboViewProjection);
	texturedShader.bind(&uboLight);

	solidShader.bind(&uboViewProjection);
	solidShader.bind(&uboLight);

	simpleShader.bind(&uboViewProjection);
	simpleShader.bind(&uboLight);

	static float radius = 0.3f;
	static float height = 1.0f;
	static int sideAmount = 3;
	MeshCylinder cylinder(texturedShader, radius, height, sideAmount, "texture_cylinder.jpg");
	MeshCone cone(texturedShader, 0.25f, 0.6f, 15, "texture_triangle.jpg");
	MeshOrbit orbit(solidShader, glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 15);
	Model nanosuit(texturedShader, "nanosuit\\nanosuit.obj"), cat(texturedShader, "cat\\cat.obj");
	GraphNode graphRoot, cylinderNode(&cylinder), coneNode(&cone), suitNode(&nanosuit), catNode(&cat), orbitNode(&orbit);
	suitNode.setScale(0.1f);
	cylinderNode.setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f)));
	coneNode.setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
	graphRoot.addChild(&cylinderNode);
	graphRoot.addChild(&coneNode);
	graphRoot.addChild(&suitNode);
	graphRoot.addChild(&catNode);
	graphRoot.addChild(&orbitNode);


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
		static glm::vec2 rotation(0.25f, 0.0f), prevRotation(0.0f, 0.0f);


		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit3("Color", (float*)&color);
			ImGui::SliderInt("Size", &targetSize, 50, SMALLER_SIZE);
			if (outputSize != targetSize) {
				outputSize = targetSize;
			}
			ImGui::NewLine();
			ImGui::SliderFloat("Cylinder radius", &targetRadius, 0.05f, 2.0f);
			ImGui::NewLine();
			ImGui::SliderFloat("Cylinder height", &targetHeight, 0.05f, 2.0f);
			ImGui::NewLine();
			ImGui::SliderInt("Cylinder sides", &targetSides, 3, 50);
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
			graphRoot.setLocal(model);
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

		glUniform1i(disableTextureLocation, shouldUseTexture ? 0 : 1);
		texturedShader.setColor(color);

		graphRoot.draw();

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

	texturedShader.remove();
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &cubeVAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}