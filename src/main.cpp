#include "UboLight.h"
#include "UboViewProjection.h"
#include <cstdio>
#include "MeshCylinder.h"
#include "MeshCone.h"
#include "MeshTorus.h"
#include "MeshSphere.h"
#include "Model.h"
#include "MeshOrbit.h"
#include "OrbitingNode.h"
#include "RotatingNode.h"
#include "GeometryShader.h"
#include "MeshCylinderGeometry.h"
#include "Camera.h"

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h> // Include glfw3.h after OpenGL definitions

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

float timeDelta = 0.0f;
bool firstMouse = true;
float lastX, lastY;
Camera camera;
bool mouseHidden = false;

bool mouseTogglePressed = false;

void process_keyboard_movement(GLFWwindow *window) {
	if (mouseHidden) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(timeDelta);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.moveBackward(timeDelta);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.moveLeft(timeDelta);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.moveRight(timeDelta);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			camera.moveDown(timeDelta);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			camera.moveUp(timeDelta);
		}
	}
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;
			case GLFW_KEY_SPACE:
				if (mouseHidden) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				} else {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					firstMouse = true;
				}
				mouseHidden = !mouseHidden;
				break;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (mouseHidden) {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera.rotateX(xoffset*timeDelta);
		camera.rotateY(yoffset*timeDelta);
	}
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

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, keyboard_callback);
	if (!mouseHidden) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	//load shaders

	Shader texturedShader("textureVertexShader.glsl", "textureFragmentShader.glsl");
	Shader solidShader("solidVertexShader.glsl", "solidFragmentShader.glsl");
	Shader simpleShader("simpleVertexShader.glsl", "simpleFragmentShader.glsl");
	GeometryShader geometryShader("cylinderVertexShader.glsl", "cylinderFragmentShader.glsl", "cylinderGeometryShader.glsl");

	glm::vec4 color(1.00f, 1.00f, 1.00f, 1.00f), lightColor(1.00f, 1.00f, 1.00f, 1.00f), clearColor(0.352f, 0.392f, 0.92f, 1.00f), prevLightColor = lightColor;
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	//MeshCylinder cylinderCode(texturedShader, 0.15f, 0.5f, 3, "texture_cylinder.jpg");

	MeshSphere sphere(texturedShader, 0.15f, 3, "texture_cylinder.jpg");
	//MeshTorus torus(texturedShader, 0.15f, 0.5f, 3, "texture_cylinder.jpg");
	MeshCone cone(texturedShader, 0.25f, 0.6f, 15, "texture_triangle.jpg");
	MeshOrbit orbit1(solidShader, glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 30), orbit2(solidShader, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 30)
		, orbit3(solidShader, glm::vec3(0.0f, 1.0f, 0.0f), 0.35f, 30), orbit4(solidShader, glm::vec3(1.0f, 1.0f, 0.0f), 0.75f, 30);
	Model nanosuit(texturedShader, "nanosuit\\nanosuit.obj"), cat(texturedShader, "cat\\cat.obj");
	//MeshCylinderGeometry cylinderGeoShader(geometryShader, radius, height, sideAmount, "texture_cylinder.jpg");
	GraphNode graphRoot;
	RotatingNode suitNode(1.0f, &nanosuit), catNode(0.6f, &cat);

	RotatingNode preVerticalNode(0.2f, NULL);
	GraphNode someNode(&sphere);
	preVerticalNode.addChild(&someNode);

	OrbitNode orbitNode1(&orbit1), orbitNode2(&orbit2), orbitNode3(&orbit3);
	OrbitingNode orbitingNode1(&orbitNode1, 0.12f, &cone), orbitingNode2(&orbitNode2, 0.09f, NULL);

	OrbitNode verticalOrbit(&orbit4);
	verticalOrbit.setLocal(glm::rotate(glm::mat4(1.0f), (float)(M_PI / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	OrbitingNode orbitingNode3(&orbitNode3, 0.15f, NULL), orbitingNode4(&verticalOrbit, 0.08f, &cat);

	suitNode.setScale(0.04f);
	catNode.setScale(0.5f);
	suitNode.setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	graphRoot.addChild(&suitNode);

	orbitingNode2.addChild(&catNode);
	orbitingNode3.addChild(&preVerticalNode);

	graphRoot.addChild(&suitNode);
	graphRoot.addChild(&orbitNode1);
	graphRoot.addChild(&orbitNode2);
	preVerticalNode.addChild(&verticalOrbit);
	orbitingNode2.addChild(&orbitNode3);

	glm::vec4 lightPosition(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SMALLER_SIZE / (float)SMALLER_SIZE, 0.1f, 100.0f);
	glm::mat4 model(1.0f);

	UboViewProjection uboViewProjection(camera.getView(), projection);
	UboLight uboLight(lightPosition, lightColor);

	texturedShader.bind(&uboViewProjection);
	texturedShader.bind(&uboLight);

	solidShader.bind(&uboViewProjection);
	solidShader.bind(&uboLight);

	simpleShader.bind(&uboViewProjection);
	simpleShader.bind(&uboLight);

	geometryShader.bind(&uboViewProjection);
	geometryShader.bind(&uboLight);


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		process_keyboard_movement(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static int outputSize = SMALLER_SIZE;
		static int targetSize = outputSize;
		static bool outline = false;
		static bool shouldUseTexture = true, shouldAutoUpdate = true;
		static glm::vec2 rotation(0.0f, 0.0f), prevRotation(0.0f, 0.0f);
		static double glfwTime;
		static double currentTime = 0.0f;
		static float timeMultiplier = 0.1f;

		static float catOrbitSpeed = 1.0f, catRotationSpeed = 1.0f;

		glfwTime = glfwGetTime();
		timeDelta = glfwTime - currentTime;
		currentTime = glfwTime;


		ImGui::Begin("Configuration");
		{
			ImGui::ColorEdit3("Color", (float*)&color);
			ImGui::SliderInt("Size", &targetSize, 50, SMALLER_SIZE);
			if (outputSize != targetSize) {
				outputSize = targetSize;
			}
			ImGui::NewLine();
			sphere.drawGui(shouldAutoUpdate);
			ImGui::NewLine();
			ImGui::Checkbox("Auto-update", &shouldAutoUpdate);
			ImGui::NewLine();
			ImGui::SliderAngle("X rotation", &(rotation.x), -180.0f, 180.0f);
			ImGui::SliderAngle("Y rotation", &(rotation.y), -180.0f, 180.0f);
			ImGui::NewLine();
			ImGui::SliderFloat("Time multiplier", &timeMultiplier, 0.0f, 10.0f);
			ImGui::SliderFloat("Cat orbiting speed", &catOrbitSpeed, 0.0f, 20.0f);
			ImGui::SliderFloat("Cat rotation speed", &catRotationSpeed, 0.0f, 20.0f);
			orbitingNode2.setOrbitSpeed(catOrbitSpeed);
			catNode.setRotationSpeed(catRotationSpeed);
			//orbitNode1.setRadius(testFloat);
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

		orbitingNode4.setScale((sin(currentTime) + 1.0f) / 2.0f);

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

		uboViewProjection.inject(camera.getView(), projection);

		texturedShader.use();
		texturedShader.setDisableTexture(!shouldUseTexture);
		texturedShader.setColor(color);
		glUseProgram(0);

		geometryShader.use();
		geometryShader.setDisableTexture(!shouldUseTexture);
		geometryShader.setColor(color);
		glUseProgram(0);

		graphRoot.update(timeDelta * timeMultiplier);
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