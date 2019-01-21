#include "MainMenuScene.h"
#include "SceneManager.h"

MainMenuScene::MainMenuScene() {
	textRenderer = SceneManager::getInstance().getTextRenderer();
	buttonNewGame = new UiTextButton(SceneManager::getInstance().getUiTextureShader(), BUTTON_LONG_IDLE,
	                                 BUTTON_LONG_HOVER, BUTTON_LONG_CLICKED,
	                                 glm::vec2(WINDOW_CENTER_X, WINDOW_HEIGHT * 0.42f)
	                                 , BUTTON_STANDARD_SIZE, SceneManager::getInstance().getTextRenderer(), "New game");
	buttonNewGame->setButtonCallback([]() { SceneManager::getInstance().newGame(); });

	glm::vec2 nextPos = buttonNewGame->getCenter();
	nextPos.y += BUTTON_STANDARD_SIZE.y + 10.0f;
	buttonOptions = new UiTextButton(SceneManager::getInstance().getUiTextureShader(), BUTTON_LONG_IDLE,
	                                 BUTTON_LONG_HOVER, BUTTON_LONG_CLICKED, nextPos
	                                 , BUTTON_STANDARD_SIZE, SceneManager::getInstance().getTextRenderer(), "Options");

	nextPos.y += BUTTON_STANDARD_SIZE.y + 10.0f;
	buttonQuit = new UiTextButton(SceneManager::getInstance().getUiTextureShader(), BUTTON_LONG_IDLE, BUTTON_LONG_HOVER,
	                              BUTTON_LONG_CLICKED, nextPos
	                              , BUTTON_STANDARD_SIZE, SceneManager::getInstance().getTextRenderer(), "Quit");
	buttonQuit->setButtonCallback([]() { SceneManager::getInstance().quitPending = true; });
}

void MainMenuScene::render() {
	textRenderer->renderText("MAIN MENU", WINDOW_CENTER_X, WINDOW_CENTER_Y / 6.0f, 2.0f, true);
	buttonNewGame->render();
	buttonOptions->render();
	buttonQuit->render();
}

void MainMenuScene::update(double timeDelta) { }

void MainMenuScene::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { }

void MainMenuScene::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	buttonNewGame->mouse_callback(window, xpos, ypos);
	buttonOptions->mouse_callback(window, xpos, ypos);
	buttonQuit->mouse_callback(window, xpos, ypos);
}

void MainMenuScene::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	buttonNewGame->mouse_button_callback(window, butt, action, mods);
	buttonOptions->mouse_button_callback(window, butt, action, mods);
	buttonQuit->mouse_button_callback(window, butt, action, mods);
}

void MainMenuScene::setOptionsCallback(std::function<void()> callback) {
	buttonOptions->setButtonCallback(callback);
}
