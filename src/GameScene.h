#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "PauseScene.h"
#include "Skybox.h"
#include "Camera.h"
#include "GraphNode.h"

class SceneManager;

class GameScene : public Scene {
public:
	GameScene();
	void render() override;
	void update(double timeDelta) override;
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) override;
protected:
	GraphNode* graphScene;
	glm::mat4 projection;
	Camera* camera;
	SceneManager* sceneManager;
	void pause();
	void unpause();
	bool paused = false;
	PauseScene* pauseScene;
	Skybox* skybox;
	bool intro = true, introHalfPassed = false;
	float introDistance = 10.0f, introDone = 0.0f, prevIntroDone = 0.0f, introSpeed = 0.0f, introAcceleration = 0.08f;
};

#endif
