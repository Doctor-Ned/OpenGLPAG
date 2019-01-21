#include "SceneManager.h"

SceneManager& SceneManager::getInstance() {
	static SceneManager *instance = nullptr;
	if(instance == nullptr) {
		instance = new SceneManager();
		instance->setup();
	}
	return *instance;
}

SceneManager::SceneManager() {
	
}

void SceneManager::setup() {
	uiTextureShader = new Shader("uiTextureVertexShader.glsl", "uiTextureFragmentShader.glsl");
	uiColorShader = new Shader("uiColorVertexShader.glsl", "uiColorFragmentShader.glsl");
	textRenderer = new TextRenderer(0.5f);
	textRenderer->load("res\\fonts\\ButterLayer.ttf", 60.0f);
	mainMenuScene = new MainMenuScene();
	optionsScene = new OptionsScene();
	mainMenuScene->setOptionsCallback([this]() {currentScene = optionsScene; });
	currentScene = mainMenuScene;
}

void SceneManager::render() {
	currentScene->render();
}

void SceneManager::update(double timeDelta) {
	currentScene->update(timeDelta);
}

void SceneManager::newGame() {
	gameScene = new GameScene();
	currentScene = gameScene;
}

void SceneManager::backToMenu() {
	currentScene = mainMenuScene;
}

TextRenderer* SceneManager::getTextRenderer() {
	return textRenderer;
}

void SceneManager::keyboard_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	currentScene->keyboard_callback(window, key, scancode, action, mods);
}

void SceneManager::mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	currentScene->mouse_callback(window, xpos, ypos);
}
void SceneManager::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	currentScene->mouse_button_callback(window, butt, action, mods);
}

Shader * SceneManager::getUiColorShader() {
	return uiColorShader;
}

Shader * SceneManager::getUiTextureShader() {
	return uiTextureShader;
}