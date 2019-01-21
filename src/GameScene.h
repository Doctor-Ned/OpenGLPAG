#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "PauseScene.h"

class GameScene : public Scene {
public:
	GameScene();
	void render() override;
	void update(double timeDelta) override;
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) override;
protected:
	void pause();
	void unpause();
	bool paused=false;
	PauseScene *pauseScene;
};

#endif