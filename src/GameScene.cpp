#include "GameScene.h"
#include "GLFW/glfw3.h"
#include "SceneManager.h"
#include "MeshColorPlane.h"
#include "BlockNode.h"
#include "DestroyableBlockNode.h"
#include <algorithm>

GameScene::GameScene() {
	sceneManager = &SceneManager::getInstance();
	pauseScene = new PauseScene();
	projection = glm::perspective(glm::radians(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
	textRenderer = sceneManager->getTextRenderer();

	std::vector<std::string> skyboxFaces{
		"res\\skybox\\lf.tga",
		"res\\skybox\\rt.tga",
		"res\\skybox\\up.tga",
		"res\\skybox\\dn.tga",
		"res\\skybox\\ft.tga",
		"res\\skybox\\bk.tga",
	};

	buttonBackToMenu = new UiTextButton(sceneManager->getUiTextureShader(), BUTTON_LONG_IDLE, BUTTON_LONG_HOVER, BUTTON_LONG_CLICKED, glm::vec2(WINDOW_CENTER_X, WINDOW_HEIGHT*0.65f),
		BUTTON_STANDARD_SIZE, textRenderer, "Return to menu");
	buttonBackToMenu->setButtonCallback([this]() {sceneManager->backToMenu(); });

	skybox = new Skybox(*sceneManager->getSkyboxShader(), skyboxFaces);
	camera = new Camera(glm::vec3(0.0f, 1.0f, 2.0f + introDistance), glm::vec3(0.0f, 0.0f, -1.0f),
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

	sceneGraph = new GraphNode(new MeshColorPlane(*sceneManager->getColorShader(), 1000.0f, 1000.0f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	int blockAmount = 8, layers = 5;
	float currentX, currentY, blockWidth;
	blockWidth = (BLOCK_MAX_X - BLOCK_MIN_X - (blockAmount - 1)*BLOCK_MARGIN) / blockAmount;
	for (int l = 0; l < layers; l++) {
		currentX = BLOCK_MIN_X;
		currentY = BLOCK_MAX_Y - l * (BLOCK_MARGIN + BLOCK_HEIGHT);
		for (int i = 0; i < blockAmount; i++) {
			blocks.push_back(new DestroyableBlockNode(new MeshColorBox(*sceneManager->getColorShader(),
				glm::vec3(currentX, currentY - BLOCK_HEIGHT, BLOCK_MIN_Z), glm::vec3(currentX + blockWidth, currentY, BLOCK_MIN_Z + BLOCK_DEPTH)
				, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)), 100, sceneGraph));
			currentX += blockWidth + BLOCK_MARGIN;
		}
	}

	movingBlockWidth = MOVINGBLOCK_BASE_WIDTH / (sceneManager->difficulty * 0.75f);

	bottomBlock = new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MIN_X, -0.1f, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MAX_X, 0.05f, BLOCK_MIN_Z + WALL_Z_ADDITION + BLOCK_DEPTH), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph);

	walls.push_back(new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MIN_X - WALL_THICKNESS, 0.0f, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MIN_X, WALL_TOP_Y - WALL_THICKNESS, BLOCK_MIN_Z + BLOCK_DEPTH + WALL_Z_ADDITION), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph));
	walls.push_back(new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MAX_X, 0.0f, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MAX_X + WALL_THICKNESS, WALL_TOP_Y - WALL_THICKNESS, BLOCK_MIN_Z + BLOCK_DEPTH + WALL_Z_ADDITION), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph));
	walls.push_back(new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MIN_X - WALL_THICKNESS, WALL_TOP_Y - WALL_THICKNESS, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MAX_X + WALL_THICKNESS, WALL_TOP_Y, BLOCK_MIN_Z + BLOCK_DEPTH + WALL_Z_ADDITION), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph));

	movingBlock = new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(-movingBlockWidth / 2.0f, MOVINGBLOCK_Y, BLOCK_MIN_Z),
		glm::vec3(movingBlockWidth / 2.0f, MOVINGBLOCK_Y + MOVINGBLOCK_HEIGHT, BLOCK_MIN_Z + BLOCK_DEPTH), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)), sceneGraph);
	movingBlockX = -movingBlockWidth / 2.0f;

	updatePoints();
}

void GameScene::render() {
	sceneManager->getUboViewProjection()->inject(camera->getView(), projection);
	sceneManager->getModelShader()->setViewPosition(camera->getPos());
	sceneManager->getTextureShader()->setViewPosition(camera->getPos());
	sceneManager->getColorShader()->setViewPosition(camera->getPos());
	sceneGraph->draw();
	skybox->draw(camera->getUntranslatedView(), projection);
	if (!intro && !paused) {
		if (!gameOver) {
			if (orb == nullptr) {
				sprintf_s(orbText, "%d orbs left", availableOrbs);
				textRenderer->renderText("Press SPACE to throw an orb.", WINDOW_CENTER_X, WINDOW_HEIGHT * 0.35f, 1.0f, true, glm::vec3(0.0f, 1.0f, 0.0f));
				textRenderer->renderText(orbText, WINDOW_CENTER_X, WINDOW_HEIGHT *0.45f, 1.0f, true, glm::vec3(1.0f, 0.0f, 0.0f));
			}
			textRenderer->renderText(pointsText, 20.0f, 20.0f, 1, false, glm::vec3(1.0f, 1.0f, 1.0f));
		} else {
			textRenderer->renderText("GAME OVER!", WINDOW_CENTER_X, WINDOW_CENTER_Y / 6.0f, 2.0f, true);
			sprintf_s(pointsText, "Finished with %d points!", points);
			textRenderer->renderText(pointsText, WINDOW_CENTER_X, WINDOW_CENTER_Y, 2.0f, true, glm::vec3(1.0f, 1.0f, 1.0f));
			buttonBackToMenu->render();
		}
	}
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
			} else {
				introSpeed += timeDelta * introAcceleration;
				introDone += introSpeed;
				if (introDone >= introDistance / 2.0f) {
					introHalfPassed = true;
				}
			}
			camera->moveForward(introDone - prevIntroDone, 1);
			prevIntroDone = introDone;
		} else if (gameOver) {
			//nothing?
		} else {
			if (movingLeft && !movingRight) {
				float target = std::max(BLOCK_MIN_X, movingBlockX - movingSpeed * (float)timeDelta);
				float diff = target - movingBlockX;
				movingBlockX = target;
				movingBlock->setLocal(glm::translate(movingBlock->getLocal(), glm::vec3(diff, 0.0f, 0.0f)));
			} else if (movingRight && !movingLeft) {
				float target = std::min(BLOCK_MAX_X - movingBlockWidth, movingBlockX + movingSpeed * (float)timeDelta);
				float diff = target - movingBlockX;
				movingBlockX = target;
				movingBlock->setLocal(glm::translate(movingBlock->getLocal(), glm::vec3(diff, 0.0f, 0.0f)));
			}
			if (orb != nullptr) {
				if (orb->actualCenter().y < movingBlock->actualMax().y + ORB_RADIUS / 2.0f || !orb->tryCollide(movingBlock)) {
					if (!orb->tryCollide(bottomBlock)) {
						bool wallBounced = false;
						for (int i = 0; i < walls.size(); i++) {
							if (orb->tryCollide(walls[i])) {
								walls[i]->onCollision(this);
								wallBounced = true;
								break;
							}
						}
						if (!wallBounced) {
							for (int i = 0; i < blocks.size(); i++) {
								if (orb->tryCollide(blocks[i])) {
									BlockNode* block = blocks[i];
									blocks.erase(blocks.begin() + i);
									block->onCollision(this);
									orb->speed += ORB_SPEED / 10.0f;
									break;
								}
							}
						}
					} else {
						loseOrb();
					}
				} else {
					movingBlock->onCollision(this);
				}
			}
			sceneGraph->update(timeDelta);
		}
	} else {
		pauseScene->update(timeDelta);
	}
}

void GameScene::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (paused) {
		pauseScene->keyboard_callback(window, key, scancode, action, mods);
	} else {
		if (key == GLFW_KEY_LEFT) {
			if (action == GLFW_PRESS) {
				movingLeft = true;
			} else if (action == GLFW_RELEASE) {
				movingLeft = false;
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (action == GLFW_PRESS) {
				movingRight = true;
			} else if (action == GLFW_RELEASE) {
				movingRight = false;
			}
		}
		if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE && !gameOver) {
			pause();
		} else if (intro) {
			if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
				intro = false;
				camera->moveForward(introDistance - introDone, 1);
			}
		} else {
			if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE && orb == nullptr) {
				orb = new OrbNode(new MeshColorSphere(*sceneManager->getColorShader(), ORB_RADIUS, 20, glm::vec4(1.0, 0.0f, 0.0f, 1.0f))
					, ORB_SPEED * sceneManager->difficulty, glm::vec2(rand() % 2 == 0 ? -1.0f : 1.0f, 1.0f), sceneGraph);
				glm::vec3 moveMin = movingBlock->actualMin(), moveMax = movingBlock->actualMax();
				orb->setLocal(glm::translate(glm::mat4(1.0f), glm::vec3((moveMin.x + moveMax.x) / 2.0f, moveMax.y + ORB_RADIUS + 0.001f, BLOCK_MIN_Z + BLOCK_DEPTH / 2.0f)));
				availableOrbs--;
			}
		}
	}
}

void GameScene::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (paused) {
		pauseScene->mouse_callback(window, xpos, ypos);
	} else if (gameOver) {
		buttonBackToMenu->mouse_callback(window, xpos, ypos);
	}
}

void GameScene::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	if (paused) {
		pauseScene->mouse_button_callback(window, butt, action, mods);
	} else if (gameOver) {
		buttonBackToMenu->mouse_button_callback(window, butt, action, mods);
	}
}

void GameScene::addPoints(int points) {
	this->points += points;
	updatePoints();
}

void GameScene::loseOrb() {
	orb->getParent()->removeChild(orb);
	delete orb;
	orb = nullptr;
	if (availableOrbs == 0) {
		gameOver = true;
	}
	//todo: play a proper sound
}

void GameScene::updatePoints() {
	sprintf_s(pointsText, "%d points", points);
}

void GameScene::pause() {
	pauseScene->setResumeCallback([this]() { unpause(); });
	paused = true;
}

void GameScene::unpause() {
	paused = false;
}
