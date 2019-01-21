#include "PauseScene.h"
#include "GLFW/glfw3.h"
#include "SceneManager.h"

PauseScene::PauseScene() {
	textRenderer = SceneManager::getInstance().getTextRenderer();
	background = new UiColorPlane(SceneManager::getInstance().getUiColorShader(), glm::vec4(0.5f, 0.5f, 0.5f, 0.2f),
	                              glm::vec2(0.0f, 0.0f), glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), false);
	buttonResume = new UiTextButton(SceneManager::getInstance().getUiTextureShader(), BUTTON_LONG_IDLE,
	                                BUTTON_LONG_HOVER, BUTTON_LONG_CLICKED,
	                                glm::vec2(WINDOW_CENTER_X, WINDOW_HEIGHT / 3.0f)
	                                , BUTTON_STANDARD_SIZE, SceneManager::getInstance().getTextRenderer(), "Resume");
	glm::vec2 menuPos = buttonResume->getCenter();
	menuPos.y += BUTTON_STANDARD_SIZE.y + 10.0f;
	buttonMenu = new UiTextButton(SceneManager::getInstance().getUiTextureShader(), BUTTON_LONG_IDLE, BUTTON_LONG_HOVER,
	                              BUTTON_LONG_CLICKED, menuPos
	                              , BUTTON_STANDARD_SIZE, SceneManager::getInstance().getTextRenderer(),
	                              "Quit to menu");
	buttonMenu->setButtonCallback([]() { SceneManager::getInstance().backToMenu(); });
}

void PauseScene::render() {
	background->render();
	textRenderer->renderText("PAUSE", WINDOW_CENTER_X, WINDOW_CENTER_Y / 6.0f, 2.0f, true);
	buttonResume->render();
	buttonMenu->render();
}

void PauseScene::update(double timeDelta) { }

void PauseScene::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		resumeCallback();
	}
}

void PauseScene::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	buttonResume->mouse_callback(window, xpos, ypos);
	buttonMenu->mouse_callback(window, xpos, ypos);
}

void PauseScene::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	buttonResume->mouse_button_callback(window, butt, action, mods);
	buttonMenu->mouse_button_callback(window, butt, action, mods);
}

void PauseScene::setResumeCallback(const std::function<void()>& resumeCallback) {
	this->resumeCallback = resumeCallback;
	buttonResume->setButtonCallback(resumeCallback);
}
