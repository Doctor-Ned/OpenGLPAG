#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "PauseScene.h"
#include "Skybox.h"
#include "Camera.h"
#include "GraphNode.h"
#include "OrbNode.h"
#include "SpotLightNode.h"
#include "MeshRefSphere.h"
#include "MeshRefBox.h"

class SceneManager;

class GameScene : public Scene {
public:
	GameScene();
	void render() override;
	void update(double timeDelta) override;
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) override;
	void addPoints(int points);
	void loseOrb();
protected:
	void generateBlocks();
	void stdRender();
	void customRender(glm::mat4 view, glm::mat4 projection, GraphNode *exclude);
	void customRender(glm::mat4 view, glm::mat4 projection, std::vector<GraphNode*> exclude);
	GLFWwindow *window;
	int points = 0;
	void updatePoints();
	const float BLOCK_HEIGHT = 0.075f, BLOCK_DEPTH = 0.1f, BLOCK_MARGIN = BLOCK_HEIGHT / 8.0f, BLOCK_MIN_X = -1.0f, BLOCK_MAX_X = 1.0f, BLOCK_MIN_Z = -1.0f, BLOCK_MAX_Y = 1.5f;
	const float MOVINGBLOCK_HEIGHT = 0.03f, MOVINGBLOCK_BASE_WIDTH = (BLOCK_MAX_X - BLOCK_MIN_X) / 8.0f, MOVINGBLOCK_Y = 0.1f;
	const float WALL_THICKNESS = 0.2f, WALL_TOP_Y = 2.0f, WALL_Z_ADDITION=0.1f;
	const float ORB_SPEED = 0.325f, ORB_RADIUS=0.035f;
	TextRenderer *textRenderer;
	char orbText[30];
	char pointsText[30];
	int availableOrbs = 3;
	BlockNode *movingBlock, *bottomBlock;
	std::vector<BlockNode*> walls;
	OrbNode *orb = nullptr;
	MeshColorSphere *orbSphere;
	std::vector<BlockNode*> blocks;
	std::vector<GraphNode*> reflectiveBlocks;
	std::vector<MeshRefBox*> reflectiveBoxes;
	std::vector<GraphNode*> refractiveBlocks;
	std::vector<MeshRefBox*> refractiveBoxes;
	std::vector<GraphNode*> excludedNodes;
	GraphNode* sceneGraph;
	UiTextButton *buttonBackToMenu;
	glm::mat4 projection;
	Camera* camera;
	SceneManager* sceneManager;
	std::vector<SpotLight*> spotLights;
	SpotLight movingBlockSpotLight;
	SpotLightNode *spotLightNode;
	void pause();
	void unpause();
	bool paused = false;
	PauseScene* pauseScene;
	Skybox* skybox;
	bool intro = true, introHalfPassed = false;
	bool gameOver = false;
	bool movingLeft = false, movingRight = false;
	float movingSpeed = 1.75f, movingBlockX, movingBlockWidth;
	float introDistance = 10.0f, introDone = 0.0f, prevIntroDone = 0.0f, introSpeed = 0.0f, introAcceleration = 0.08f;

	float camSpeed = 3.0f, camRotSpeed=0.5f;

	float lastMouseX, lastMouseY;
	bool freeCam = false, measureMouse;
	bool camLeft = false, camRight=false, camForward=false, camBackward=false, camUp=false, camDown=false;
};

#endif
