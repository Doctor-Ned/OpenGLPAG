#include "Headers.h"
#include <cstdio>
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h> // Include glfw3.h after OpenGL definitions
#include "freetype/ftparams.h"
#include "TextRenderer.h"
#include "UiButton.h"
#include "UiTextButton.h"
#include "UiSlider.h"
#include "SceneManager.h"

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static SceneManager *sceneManager;

void process_keyboard_movement(GLFWwindow *window) {

}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	sceneManager->keyboard_callback(window, key, scancode, action, mods);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	sceneManager->mouse_callback(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	sceneManager->mouse_button_callback(window, butt, action, mods);
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

	glEnable(GL_DEPTH_TEST);   // this is so important. Spent 2 hours looking for it
	//glAlphaFunc(GL_GREATER, 0.1f);
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	sceneManager = &SceneManager::getInstance();

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, keyboard_callback);

	glm::vec3 xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1);

	//const int SMALLER_SIZE = WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH;
	glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window) && !sceneManager->quitPending) {
		glfwPollEvents();
		process_keyboard_movement(window);
		// Rendering
		static double currentTime, lastTime = 0.0, timeDelta;
		currentTime = glfwGetTime();
		timeDelta = currentTime - lastTime;
		lastTime = currentTime;
		sceneManager->update(timeDelta);
		static int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sceneManager->render();
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}