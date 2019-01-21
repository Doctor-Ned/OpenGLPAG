#include "GameScene.h"
#include "GLFW/glfw3.h"

GameScene::GameScene() {
	pauseScene = new PauseScene();
}

void GameScene::render() {

	if (paused) {
		pauseScene->render();
	}
}

void GameScene::update(double timeDelta) {
	if (!paused) {

	} else {
		pauseScene->update(timeDelta);
	}
}

void GameScene::keyboard_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (paused) {
		pauseScene->keyboard_callback(window, key, scancode, action, mods);
	} else {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
			pause();
		}
	}
}

void GameScene::mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	if (paused) {
		pauseScene->mouse_callback(window, xpos, ypos);
	} else {
		
	}
}
void GameScene::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	if (paused) {
		pauseScene->mouse_button_callback(window, butt, action, mods);
	} else {
		
	}
}

void GameScene::pause() {
	pauseScene->setResumeCallback([this]() {unpause(); });
	paused = true;
}

void GameScene::unpause() {
	paused = false;
}
