#include "UboLight.h"
#include "UboDirLights.h"
#include "UboPointLights.h"
#include "UboSpotLights.h"
#include "UboViewProjection.h"
#include "UboTextureColor.h"
#include <cstdio>
#include "MeshCylinder.h"
#include "MeshCone.h"
#include "MeshTorus.h"
#include "MeshSphere.h"
#include "Model.h"
#include "ModelInstanced.h"
#include "MeshOrbit.h"
#include "OrbitingNode.h"
#include "RotatingNode.h"
#include "GeometryShader.h"
#include "MeshCylinderGeometry.h"
#include "Camera.h"
#include "DirLightNode.h"
#include "PointLightNode.h"
#include "SpotLightNode.h"
#include "MeshPlane.h"
#include "Skybox.h"

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
		float speed = timeDelta;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			speed *= 3.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			speed *= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.moveBackward(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.moveLeft(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.moveRight(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			camera.moveDown(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			camera.moveUp(speed);
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


	glm::vec4 lightPosition(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1);

	//load shaders

	//Shader modelShader("modelVertexShader.glsl", "modelFragmentShader.glsl");
	Shader modelBPShader("modelBPVertexShader.glsl", "modelBPFragmentShader.glsl");
	Shader instanceModelShader("instanceModelVertexShader.glsl", "modelBPFragmentShader.glsl");

	std::vector<std::string> skyboxFaces{
		"hw_nightsky\\nightsky_lf.tga",
		"hw_nightsky\\nightsky_rt.tga",
		"hw_nightsky\\nightsky_up.tga",
		"hw_nightsky\\nightsky_dn.tga",
		"hw_nightsky\\nightsky_ft.tga",
		"hw_nightsky\\nightsky_bk.tga",
	};

	Shader skyboxShader("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");
	Skybox skybox(skyboxShader, skyboxFaces);

	std::vector<Shader> updatableShaders;

	//updatableShaders.push_back(modelShader);
	updatableShaders.push_back(modelBPShader);
	updatableShaders.push_back(instanceModelShader);

	glm::vec4 color(1.00f, 1.00f, 1.00f, 1.00f), clearColor(0.352f, 0.392f, 0.92f, 1.00f), lightColor(1.00f, 1.00f, 1.00f, 1.00f), prevLightColor = lightColor;
	const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;

	GraphNode graphRoot;

	int width = 5, length = 8, offsetSize = width * length;
	glm::vec3 *offsets = createHorizontalTransformArray(width, length, glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, 0.5f), 0.0f);

	std::vector<DirLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	DirLight dir1;
	dir1.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	dir1.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	dir1.diffuse = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
	dir1.direction = glm::normalize(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
	dir1.model = glm::mat4(1.0f);

	dirLights.push_back(&dir1);

	PointLight point1, point2;
	point1.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	point1.diffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	point1.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	point1.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	point1.model = glm::mat4(1.0f);
	point1.constant = 0.38f;
	point1.linear = 0.42f;
	point1.quadratic = 0.18f;

	point2 = point1;

	point2.diffuse = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);

	pointLights.push_back(&point1);
	pointLights.push_back(&point2);

	SpotLight spot1, spot2;
	spot1.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	spot1.diffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	spot1.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	spot1.position = glm::vec4(0.95f, 0.95f, 0.95f, 1.0f);
	spot1.direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	spot1.model = glm::mat4(1.0f);
	spot1.constant = 0.18f;
	spot1.linear = 0.1f;
	spot1.quadratic = 0.1f;
	spot1.cutOff = glm::radians(15.0f);
	spot1.outerCutOff = glm::radians(28.0f);

	*(&spot2) = *(&spot1);

	spot2.diffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	spot2.position = glm::vec4(0.95f, 1.67f, -0.48f, 1.0f);
	spot2.direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	spot2.constant = 0.18f;
	spot2.linear = 0.1f;
	spot2.quadratic = 0.1f;
	spot2.cutOff = glm::radians(6.0f);
	spot2.outerCutOff = glm::radians(19.0f);

	spotLights.push_back(&spot1);
	spotLights.push_back(&spot2);

	//ModelInstanced nanosuitInstance(instanceModelShader, "nanosuit\\nanosuit.obj", offsets, offsetSize);
	Model nanosuit(modelBPShader, "nanosuit\\nanosuit.obj");
	glm::vec3 *cabinOffsets = createHorizontalTransformArray(14, 7, glm::vec2(-8.0f, -12.0f), glm::vec2(8.0f, 12.0f), 0.0f);
	ModelInstanced cabin(instanceModelShader, "cabin\\cabin.obj", cabinOffsets, 14 * 7);
	Model cat(modelBPShader, "cat\\cat.obj");

	MeshSphere pointMesh(modelBPShader, 0.05f, 20, "red.jpg");
	pointMesh.setUseLight(false);

	MeshCone reflector(modelBPShader, 0.05f, 0.2f, 20, "red.jpg");
	reflector.setUseLight(false);

	MeshPlane plane(modelBPShader, 40.0f, 40.0f, "grass.jpg");
	//plane.setUseLight(false);

	GraphNode suitNode(&nanosuit, &graphRoot);
	suitNode.setScale(0.05f);

	GraphNode catNode(&cat, &graphRoot);
	catNode.setScale(0.55f);
	catNode.setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -1.3f)));

	GraphNode cabins(&cabin, &graphRoot);
	cabins.setScale(0.01f);

	DirLightNode dirNode(&dir1, NULL, &graphRoot);

	OrbitNode orbit(NULL, &graphRoot);
	orbit.setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	OrbitingNode orb1(&orbit, 5.0f), orb2(&orbit, 5.0f);
	orb2.setAngle(glm::radians(180.0f));

	PointLightNode point1Node(&point1, &pointMesh, &orb1);
	PointLightNode point2Node(&point2, &pointMesh, &orb2);




	SpotLightNode spot1Node(&spot1, NULL, &graphRoot);
	spot1Node.rotationZ = glm::radians(43.0f);
	spot1Node.rotationX = glm::radians(38.0f);
	SpotLightNode spot2Node(&spot2, NULL, &graphRoot);
	spot2Node.rotationZ = glm::radians(-13.0f);
	spot2Node.rotationX = glm::radians(23.0f);
	GraphNode refl1Node(&reflector, &spot1Node);
	GraphNode refl2Node(&reflector, &spot2Node);

	refl1Node.setLocal(glm::rotate(glm::mat4(1.0f), (float)M_PI, zAxis));
	refl2Node.setLocal(glm::rotate(glm::mat4(1.0f), (float)M_PI, zAxis));

	GraphNode planeNode(&plane, &graphRoot);


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SMALLER_SIZE / (float)SMALLER_SIZE, 0.1f, 100.0f);
	glm::mat4 model(1.0f);

	std::vector<Mesh*> updatableMeshes;
	updatableMeshes.push_back(&nanosuit);
	updatableMeshes.push_back(&plane);

	UboViewProjection uboViewProjection(camera.getView(), projection);
	UboLight uboLight(lightPosition, lightColor);
	UboTextureColor uboTextureColor(false, color);
	int dirSize = dirLights.size(), pointSize = pointLights.size(), spotSize = spotLights.size();
	DirLight **dirPointer = dirSize == 0 ? NULL : &dirLights[0];
	PointLight **pointPointer = pointSize == 0 ? NULL : &pointLights[0];
	SpotLight **spotPointer = spotSize == 0 ? NULL : &spotLights[0];
	UboDirLights uboDirLights(dirSize, dirPointer);
	UboPointLights uboPointLights(pointSize, pointPointer);
	UboSpotLights uboSpotLights(spotSize, spotPointer);
	//UboLights uboLights(dirSize, pointSize, spotSize, dirPointer, pointPointer, spotPointer);

	for (int i = 0; i < updatableShaders.size(); i++) {
		updatableShaders[i].bind(&uboViewProjection);
		updatableShaders[i].bind(&uboLight);
		updatableShaders[i].bind(&uboDirLights);
		updatableShaders[i].bind(&uboPointLights);
		updatableShaders[i].bind(&uboSpotLights);
		updatableShaders[i].bind(&uboTextureColor);
	}

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
		static int shader = 0;
		static int targetShader = shader + 1;
		static float shininess = 16.0f;
		static bool blinnPhong = true;

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
			ImGui::Checkbox("Auto-update", &shouldAutoUpdate);
			ImGui::NewLine();
			dirNode.drawGui(shouldAutoUpdate);
			point1Node.drawGui(shouldAutoUpdate);
			point2Node.drawGui(shouldAutoUpdate);
			spot1Node.drawGui(shouldAutoUpdate);
			spot2Node.drawGui(shouldAutoUpdate);
			//ImGui::SliderAngle("X rotation", &(rotation.x), -180.0f, 180.0f);
			ImGui::SliderAngle("Y rotation", &(rotation.y), -180.0f, 180.0f);
			ImGui::NewLine();
			ImGui::SliderFloat("Time multiplier", &timeMultiplier, 0.0f, 10.0f);
			ImGui::NewLine();
			ImGui::SliderFloat("Shininess", &shininess, 0.0f, 128.0f);
			ImGui::NewLine();
			//ImGui::SliderInt("Shader", &targetShader, 0, 1);
			//ImGui::NewLine();
			ImGui::Checkbox("Use texture", &shouldUseTexture);
			ImGui::NewLine();
			ImGui::Checkbox("Blinn Phong (Lambert otherwise)", &blinnPhong);
			ImGui::NewLine();
			//ImGui::ColorEdit3("Light color", (float*)&dir1.diffuse);
			//ImGui::NewLine();
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

		//dir1.direction = glm::rotate(dir1.direction, 0.008f, glm::vec3(0.0f, 1.0f, 0.0f));

		if (shader != targetShader) {
			shader = targetShader;
			Shader *s = NULL;
			switch (shader) {
				case 0:
					s = &modelBPShader;
					break;
			}
			if (s != NULL) {
				for (int i = 0; i < updatableMeshes.size(); i++) {
					updatableMeshes[i]->setShader(*s);
				}
			}
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

		uboViewProjection.inject(camera.getView(), projection);
		uboTextureColor.inject(!shouldUseTexture, color);
		uboDirLights.inject(dirSize, dirPointer);
		uboPointLights.inject(pointSize, pointPointer);
		uboSpotLights.inject(spotSize, spotPointer);


		for (int i = 0; i < updatableShaders.size(); i++) {
			updatableShaders[i].setViewPosition(camera.getPos());
			updatableShaders[i].setShininess(shininess);
			updatableShaders[i].setBlinnPhong(blinnPhong);
		}

		graphRoot.update(timeDelta * timeMultiplier);
		graphRoot.draw();

		if (outline)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		skybox.draw(camera.getUntranslatedView(), projection);

		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for (int i = 0; i < updatableShaders.size(); i++) {
		updatableShaders[i].remove();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}