#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "MainMenuScene.h"
#include "OptionsScene.h"
#include "GameScene.h"

class SceneManager {
public:
	static SceneManager& getInstance();
	void render();
	void update(double timeDelta);
	void newGame();
	void backToMenu();
	TextRenderer *getTextRenderer();
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods);
	bool quitPending = false;
	Shader *getUiColorShader();
	Shader *getUiTextureShader();
	float difficulty = 1.0f;
	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;
protected:
	SceneManager();
	void setup();
	Shader *uiColorShader, *uiTextureShader;
	TextRenderer *textRenderer;
	Scene *currentScene = nullptr;
	MainMenuScene *mainMenuScene;
	OptionsScene *optionsScene;
	GameScene *gameScene;
};

#endif