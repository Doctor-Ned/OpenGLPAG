#include "GameScene.h"
#include "GLFW/glfw3.h"
#include "SceneManager.h"
#include "MeshColorPlane.h"

GameScene::GameScene() {
	sceneManager = &SceneManager::getInstance();
	pauseScene = new PauseScene();
	projection = glm::perspective(glm::radians(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
	std::vector<std::string> skyboxFaces{
		"res\\skybox\\lf.tga",
		"res\\skybox\\rt.tga",
		"res\\skybox\\up.tga",
		"res\\skybox\\dn.tga",
		"res\\skybox\\ft.tga",
		"res\\skybox\\bk.tga",
	};
	skybox = new Skybox(*sceneManager->getSkyboxShader(), skyboxFaces);
	camera = new Camera(glm::vec3(0.0f, 1.0f, 1.0f + introDistance), glm::vec3(0.0f, 0.0f, -1.0f),
	                    glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f);

	DirLight dir1;
	dir1.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	dir1.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	dir1.diffuse = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	dir1.direction = normalize(glm::vec4(0.0f, -1.0f, -1.0f, 1.0f));
	dir1.model = glm::mat4(1.0f);

	std::vector<DirLight*> dirs;
	dirs.push_back(&dir1);

	sceneManager->getUboDirLights()->inject(1, &dirs[0]);

	graphScene = new GraphNode(new MeshColorPlane(*sceneManager->getColorShader(), 1000.0f, 1000.0f,
	                                              glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	GraphNode* test = new GraphNode(new MeshColorPlane(*sceneManager->getColorShader(), 2.0f, 2.0f,
	                                                   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
	                                graphScene);
	test->getMesh()->setUseLight(false);
	test->setLocal(rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
}

void GameScene::render() {
	sceneManager->getUboViewProjection()->inject(camera->getView(), projection);
	sceneManager->getModelShader()->setViewPosition(camera->getPos());
	sceneManager->getTextureShader()->setViewPosition(camera->getPos());
	sceneManager->getColorShader()->setViewPosition(camera->getPos());
	graphScene->draw();
	skybox->draw(camera->getUntranslatedView(), projection);
	if (paused) {
		pauseScene->render();
	}
}

void GameScene::update(double timeDelta) {
	if (!paused) {
		if (intro) {
			if (introHalfPassed) {
				introSpeed -= timeDelta * introAcceleration;
				introDone += introSpeed;
				if (introDone >= introDistance) {
					introDone = introDistance;
					intro = false;
				}
			}
			else {
				introSpeed += timeDelta * introAcceleration;
				introDone += introSpeed;
				if (introDone >= introDistance / 2.0f) {
					introHalfPassed = true;
				}
			}
			camera->moveForward(introDone - prevIntroDone, 1);
			prevIntroDone = introDone;
		}
		else {
			graphScene->update(timeDelta);
		}
	}
	else {
		pauseScene->update(timeDelta);
	}
}

void GameScene::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (paused) {
		pauseScene->keyboard_callback(window, key, scancode, action, mods);
	}
	else {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
			pause();
		}
		else if (!intro) { }
	}
}

void GameScene::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (paused) {
		pauseScene->mouse_callback(window, xpos, ypos);
	}
	else if (!intro) { }
}

void GameScene::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	if (paused) {
		pauseScene->mouse_button_callback(window, butt, action, mods);
	}
	else if (!intro) { }
}

void GameScene::pause() {
	pauseScene->setResumeCallback([this]() { unpause(); });
	paused = true;
}

void GameScene::unpause() {
	paused = false;
}
