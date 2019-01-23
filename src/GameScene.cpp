#include "GameScene.h"
#include "GLFW/glfw3.h"
#include "SceneManager.h"
#include "MeshColorPlane.h"
#include "BlockNode.h"
#include "DestroyableBlockNode.h"
#include <algorithm>
#include "RotatingNode.h"
#include "Model.h"

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
	const float orthoSize = 5.0f;

	dirLightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, DIR_LIGHT_PROJ_NEAR, DIR_LIGHT_PROJ_FAR);

	sceneGraph = new GraphNode();

	GraphNode *ground = new GraphNode(new MeshColorPlane(*sceneManager->getColorShader(), 1000.0f, 1000.0f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), sceneGraph);

	//ground->setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)));

	GraphNode *rotatingNode = new RotatingNode(0.001f, nullptr, sceneGraph);

	//DirLight dir1;
	//dirPosition = glm::vec3(0.0f, 3.0f, 3.0f);
	dirLight.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLight.diffuse = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLight.direction = normalize(glm::vec4(0.0f, -1.0f, -1.0f, 1.0f));
	//dir1.model = glm::translate(glm::mat4(1.0f), dirPosition);
	dirLight.model = glm::mat4(1.0f);
	dirLightNode = new DirLightNode(&dirLight, nullptr, rotatingNode);
	dirLightNode->setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 3.0f)));

	orbSphere = new MeshColorSphere(*sceneManager->getColorShader(), ORB_RADIUS, 50, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	spotLight.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	spotLight.diffuse = glm::vec4(0.0f, 0.0f, 0.6f, 1.0f);
	spotLight.specular = glm::vec4(0.0f, 0.0f, 0.6f, 1.0f);
	spotLight.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	spotLight.direction = glm::normalize(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spotLight.model = glm::mat4(1.0f);
	spotLight.constant = 0.18f;
	spotLight.linear = 0.1f;
	spotLight.quadratic = 0.1f;
	spotLight.cutOff = glm::radians(12.5f);
	spotLight.outerCutOff = glm::radians(25.0f);

	spotLightProjection = glm::perspective(glm::radians(45.0f), 1.0f, SPOT_LIGHT_PROJ_NEAR, SPOT_LIGHT_PROJ_FAR);
	//spotLightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.01f, 7.5f);

	pointLight.model = glm::mat4(1.0f);
	pointLight.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	pointLight.diffuse = glm::vec4(0.3f, 0.3f, 0.1f, 1.0f);
	pointLight.specular = glm::vec4(0.3f, 0.3f, 0.1f, 1.0f);
	pointLight.position = glm::vec4(0.0f, 2.0f, -5.0f, 1.0f);
	pointLight.model = glm::mat4(1.0f);
	pointLight.constant = 0.18f;
	pointLight.linear = 0.1f;
	pointLight.quadratic = 0.1f;
	sceneManager->getUboLights()->inject(&dirLight, &spotLight, &pointLight);

	//GraphNode *backgroundObjects = new GraphNode(new Model(*sceneManager->getModelShader(), "res\\models\\ruins_final\\ruins_final.obj"), sceneGraph);
	//backgroundObjects->setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 0.0f, -10.0f)));
	//backgroundObjects->setScale(0.04f);

	GraphNode *background = new GraphNode(new MeshColorPlane(*sceneManager->getColorShader(), 1.0f, 20.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)), sceneGraph);
	background->setLocal(glm::translate(glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.0f, -6.0f, 0.0f)));

	generateBlocks();

	movingBlockWidth = MOVINGBLOCK_BASE_WIDTH / (sceneManager->difficulty * 0.75f);

	bottomBlock = new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MIN_X - WALL_THICKNESS, -0.1f, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MAX_X + WALL_THICKNESS, 0.05f, BLOCK_MIN_Z + WALL_Z_ADDITION + BLOCK_DEPTH), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph);

	walls.push_back(new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MIN_X - WALL_THICKNESS, 0.0f, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MIN_X, WALL_TOP_Y - WALL_THICKNESS, BLOCK_MIN_Z + BLOCK_DEPTH + WALL_Z_ADDITION), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph));
	walls.push_back(new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MAX_X, 0.0f, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MAX_X + WALL_THICKNESS, WALL_TOP_Y - WALL_THICKNESS, BLOCK_MIN_Z + BLOCK_DEPTH + WALL_Z_ADDITION), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph));
	walls.push_back(new BlockNode(new MeshColorBox(*sceneManager->getColorShader(), glm::vec3(BLOCK_MIN_X - WALL_THICKNESS, WALL_TOP_Y - WALL_THICKNESS, BLOCK_MIN_Z - WALL_Z_ADDITION)
		, glm::vec3(BLOCK_MAX_X + WALL_THICKNESS, WALL_TOP_Y, BLOCK_MIN_Z + BLOCK_DEPTH + WALL_Z_ADDITION), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), sceneGraph));

	movingBlock = new BlockNode(new MeshBox(*sceneManager->getTextureShader(), glm::vec3(-movingBlockWidth / 2.0f, MOVINGBLOCK_Y, BLOCK_MIN_Z),
		glm::vec3(movingBlockWidth / 2.0f, MOVINGBLOCK_Y + MOVINGBLOCK_HEIGHT, BLOCK_MIN_Z + BLOCK_DEPTH), "res\\textures\\BlockRed.png"), sceneGraph);
	movingBlockX = -movingBlockWidth / 2.0f;

	spotLightNode = new SpotLightNode(&spotLight, nullptr, movingBlock);
	spotLightNode->setLocal(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, MOVINGBLOCK_Y + MOVINGBLOCK_HEIGHT + 0.001f, BLOCK_MIN_Z + BLOCK_DEPTH / 2.0f)));

	glGenFramebuffers(1, &dirFbo);
	glGenTextures(1, &dirTexture);
	glBindTexture(GL_TEXTURE_2D, dirTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, dirFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dirTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneManager->getFramebuffer());

	glGenFramebuffers(1, &spotFbo);
	glGenTextures(1, &spotTexture);
	glBindTexture(GL_TEXTURE_2D, spotTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, spotFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, spotTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneManager->getFramebuffer());

	glGenFramebuffers(1, &pointFbo);
	glGenTextures(1, &pointTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pointTexture);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, pointFbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, POINT_LIGHT_PROJ_NEAR, POINT_LIGHT_PROJ_FAR);
	pointSpaces.push_back(shadowProj * glm::lookAt(glm::vec3(pointLight.position), glm::vec3(pointLight.position) + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	pointSpaces.push_back(shadowProj * glm::lookAt(glm::vec3(pointLight.position), glm::vec3(pointLight.position) + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	pointSpaces.push_back(shadowProj * glm::lookAt(glm::vec3(pointLight.position), glm::vec3(pointLight.position) + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	pointSpaces.push_back(shadowProj * glm::lookAt(glm::vec3(pointLight.position), glm::vec3(pointLight.position) + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	pointSpaces.push_back(shadowProj * glm::lookAt(glm::vec3(pointLight.position), glm::vec3(pointLight.position) + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	pointSpaces.push_back(shadowProj * glm::lookAt(glm::vec3(pointLight.position), glm::vec3(pointLight.position) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	updatableShaders.push_back(sceneManager->getModelShader());
	updatableShaders.push_back(sceneManager->getTextureShader());
	updatableShaders.push_back(sceneManager->getColorShader());
	updatableShaders.push_back(sceneManager->getReflectShader());
	updatableShaders.push_back(sceneManager->getRefractShader());

	updatePoints();
}

void GameScene::render() {
	sceneManager->getUboLights()->inject(&dirLight, &spotLight, &pointLight);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glCullFace(GL_FRONT);
	dirRender();
	spotRender();
	pointRender();
	//glCullFace(GL_BACK);

	for (int i = 0; i < updatableShaders.size(); i++) {
		updatableShaders[i]->use();
		updatableShaders[i]->setFloat("far_plane", POINT_LIGHT_PROJ_FAR);
		updatableShaders[i]->setDirLightSpace(dirSpace);
		updatableShaders[i]->setSpotLightSpace(spotSpace);
		updatableShaders[i]->setPointLightSpace(pointSpace);
		glActiveTexture(GL_TEXTURE31);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pointTexture);
		updatableShaders[i]->setInt("point_shadows", 31);
		glActiveTexture(GL_TEXTURE29);
		glBindTexture(GL_TEXTURE_2D, dirTexture);
		updatableShaders[i]->setInt("dir_shadows", 29);
		glActiveTexture(GL_TEXTURE30);
		glBindTexture(GL_TEXTURE_2D, spotTexture);
		updatableShaders[i]->setInt("spot_shadows", 30);
	}

	for (int i = 0; i < reflectiveBoxes.size(); i++) {
		reflectiveBoxes[i]->regenEnvironmentMap(reflectiveBlocks[i]->getWorld(), [this](glm::mat4 view, glm::mat4 projection) {
			customRender(view, projection, excludedNodes);
		}, sceneManager->getFramebuffer());
	}
	for (int i = 0; i < refractiveBoxes.size(); i++) {
		refractiveBoxes[i]->regenEnvironmentMap(refractiveBlocks[i]->getWorld(), [this](glm::mat4 view, glm::mat4 projection) {
			customRender(view, projection, excludedNodes);
		}, sceneManager->getFramebuffer());
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	stdRender();

	if (!intro && !paused && !showDepthMap) {
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

	if (showDepthMap && !paused && !gameOver) {

		//sceneManager->getDepthDebugShader()->setInt("perspective", 0);
		//sceneManager->getDepthDebugShader()->setFloat("near_plane", DIR_LIGHT_PROJ_NEAR);
		//sceneManager->getDepthDebugShader()->setFloat("far_plane", DIR_LIGHT_PROJ_FAR);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, dirTexture);

		sceneManager->getDepthDebugShader()->setInt("perspective", 1);
		sceneManager->getDepthDebugShader()->setFloat("near_plane", SPOT_LIGHT_PROJ_NEAR);
		sceneManager->getDepthDebugShader()->setFloat("far_plane", SPOT_LIGHT_PROJ_FAR);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spotTexture);

		static unsigned int quadVAO = 0;
		static unsigned int quadVBO;
		if (quadVAO == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glUseProgram(0);
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
			if (freeCam) {
				if (camForward && !camBackward) {
					camera->moveForward(camSpeed*timeDelta);
				} else if (camBackward && !camForward) {
					camera->moveBackward(camSpeed*timeDelta);
				}
				if (camLeft && !camRight) {
					camera->moveLeft(camSpeed*timeDelta);
				} else if (camRight && !camLeft) {
					camera->moveRight(camSpeed*timeDelta);
				}
				if (camUp && !camDown) {
					camera->moveUp(camSpeed*timeDelta);
				} else if (camDown && !camUp) {
					camera->moveDown(camSpeed*timeDelta);
				}
			}
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
									for (int j = 0; j < reflectiveBlocks.size(); j++) {
										if (reflectiveBlocks[j] == block) {
											reflectiveBlocks.erase(reflectiveBlocks.begin() + j);
											reflectiveBoxes.erase(reflectiveBoxes.begin() + j);
											break;
										}
									}
									for (int j = 0; j < refractiveBlocks.size(); j++) {
										if (refractiveBlocks[j] == block) {
											refractiveBlocks.erase(refractiveBlocks.begin() + j);
											refractiveBoxes.erase(refractiveBoxes.begin() + j);
											break;
										}
									}
									block->onCollision(this);
									orb->speed += ORB_SPEED / 10.0f;
									if (blocks.size() == 0) {
										availableOrbs++;
										addPoints(1000);
										//TODO: play some sound
										loseOrb();
										generateBlocks();
									}
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
	this->window = window;
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

		if (key == GLFW_KEY_W) {
			if (action == GLFW_PRESS) {
				camForward = true;
			} else if (action == GLFW_RELEASE) {
				camForward = false;
			}
		}
		if (key == GLFW_KEY_S) {
			if (action == GLFW_PRESS) {
				camBackward = true;
			} else if (action == GLFW_RELEASE) {
				camBackward = false;
			}
		}
		if (key == GLFW_KEY_A) {
			if (action == GLFW_PRESS) {
				camLeft = true;
			} else if (action == GLFW_RELEASE) {
				camLeft = false;
			}
		}
		if (key == GLFW_KEY_D) {
			if (action == GLFW_PRESS) {
				camRight = true;
			} else if (action == GLFW_RELEASE) {
				camRight = false;
			}
		}
		if (key == GLFW_KEY_Q) {
			if (action == GLFW_PRESS) {
				camDown = true;
			} else if (action == GLFW_RELEASE) {
				camDown = false;
			}
		}
		if (key == GLFW_KEY_E) {
			if (action == GLFW_PRESS) {
				camUp = true;
			} else if (action == GLFW_RELEASE) {
				camUp = false;
			}
		}

		if (key == GLFW_KEY_M && action == GLFW_RELEASE) {
			showDepthMap = !showDepthMap;
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE && !gameOver) {
			pause();
		} else if (intro) {
			if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
				intro = false;
				camera->moveForward(introDistance - introDone, 1);
			}
		} else {
			if (key == GLFW_KEY_C && action == GLFW_RELEASE && !gameOver) {
				freeCam = !freeCam;
				if (freeCam) {
					measureMouse = true;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				} else {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}

			if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE && orb == nullptr && availableOrbs > 0) {
				orb = new OrbNode(orbSphere, ORB_SPEED * sceneManager->difficulty, glm::vec2(rand() % 2 == 0 ? -1.0f : 1.0f, 1.0f), sceneGraph);
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
	} else if (freeCam) {
		if (measureMouse) {
			lastMouseX = xpos;
			lastMouseY = ypos;
			measureMouse = false;
		} else {
			camera->rotateX(camRotSpeed * (xpos - lastMouseX));
			camera->rotateY(camRotSpeed * (lastMouseY - ypos));
			lastMouseX = xpos;
			lastMouseY = ypos;
		}
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
		freeCam = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	//todo: play a proper sound
}

void GameScene::generateBlocks() {
	int blockAmount = 8, layers = 7;
	float currentX, currentY, blockWidth;
	blockWidth = (BLOCK_MAX_X - BLOCK_MIN_X - (blockAmount - 1)*BLOCK_MARGIN) / blockAmount;
	for (int l = 0; l < layers; l++) {
		currentX = BLOCK_MIN_X;
		currentY = BLOCK_MAX_Y - l * (BLOCK_MARGIN + BLOCK_HEIGHT);
		for (int i = 0; i < blockAmount; i++) {
			if (l == 0) {
				if (i > 2 && i < blockAmount - 3) {
					MeshRefBox *refBox = new MeshRefBox(*sceneManager->getReflectShader(),
						glm::vec3(currentX, currentY - BLOCK_HEIGHT, BLOCK_MIN_Z), glm::vec3(currentX + blockWidth, currentY, BLOCK_MIN_Z + BLOCK_DEPTH));
					reflectiveBoxes.push_back(refBox);
					DestroyableBlockNode *node = new DestroyableBlockNode(refBox, 500, sceneGraph);
					reflectiveBlocks.push_back(node);
					excludedNodes.push_back(node);
					blocks.push_back(node);
				}
			} else  if (l == 1) {
				if (i > 1 && i < blockAmount - 2) {
					MeshRefBox *refBox = new MeshRefBox(*sceneManager->getRefractShader(),
						glm::vec3(currentX, currentY - BLOCK_HEIGHT, BLOCK_MIN_Z), glm::vec3(currentX + blockWidth, currentY, BLOCK_MIN_Z + BLOCK_DEPTH));
					refractiveBoxes.push_back(refBox);
					DestroyableBlockNode *node = new DestroyableBlockNode(refBox, 250, sceneGraph);
					refractiveBlocks.push_back(node);
					excludedNodes.push_back(node);
					blocks.push_back(node);
				}
			} else {
				blocks.push_back(new DestroyableBlockNode(new MeshBox(*sceneManager->getTextureShader(),
					glm::vec3(currentX, currentY - BLOCK_HEIGHT, BLOCK_MIN_Z), glm::vec3(currentX + blockWidth, currentY, BLOCK_MIN_Z + BLOCK_DEPTH)
					, "res\\textures\\BlockBlue.png"), 100, sceneGraph));
			}
			currentX += blockWidth + BLOCK_MARGIN;
		}
	}
}

void GameScene::stdRender() {
	sceneManager->getUboViewProjection()->inject(camera->getView(), projection);
	for (int i = 0; i < updatableShaders.size(); i++) {
		updatableShaders[i]->setViewPosition(camera->getPos());
	}
	sceneGraph->draw();
	skybox->draw(camera->getUntranslatedView(), projection);
	//skybox->draw(camera->getUntranslatedView(), projection, pointTexture);
}

void GameScene::dirRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, dirFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	sceneManager->getDepthShader()->use();
	glm::vec3 pos = glm::vec3(dirLightNode->getWorld()[3]);
	dirSpace = dirLightProjection * glm::lookAt(pos - glm::normalize(glm::vec3(dirLightNode->getWorld() * glm::vec4(glm::vec3(dirLight.direction), 0.0f))), pos, glm::vec3(0.0f, 1.0f, 0.0f));
	sceneManager->getDepthShader()->setLightSpace(dirSpace);
	sceneGraph->draw(sceneManager->getDepthShader());
	glBindFramebuffer(GL_FRAMEBUFFER, sceneManager->getFramebuffer());
}

void GameScene::spotRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, spotFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	sceneManager->getDepthShader()->use();
	spotSpace = spotLightProjection * glm::lookAt(glm::vec3(spotLightNode->getWorld()[3]),
		glm::vec3(spotLightNode->getWorld()[3]) + glm::normalize(glm::vec3(spotLightNode->getWorld() * glm::vec4(glm::vec3(spotLight.direction), 0.0f))), glm::vec3(0.0f, 0.0f, 1.0f));
	sceneManager->getDepthShader()->setLightSpace(spotSpace);
	sceneGraph->draw(sceneManager->getDepthShader());
	glBindFramebuffer(GL_FRAMEBUFFER, sceneManager->getFramebuffer());
}

void GameScene::pointRender() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, pointFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	GeometryShader *shader = sceneManager->getDepthPointShader();
	shader->setFloat("far_plane", POINT_LIGHT_PROJ_FAR);
	shader->setPointPosition(pointLight.model * pointLight.position);
	shader->setPointSpaces(pointSpaces);
	sceneGraph->draw(shader);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneManager->getFramebuffer());
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}

void GameScene::customRender(glm::mat4 view, glm::mat4 projection, GraphNode* exclude) {
	sceneManager->getUboViewProjection()->inject(view, projection);
	for (int i = 0; i < updatableShaders.size(); i++) {
		updatableShaders[i]->setViewPosition(view[3]);
	}
	sceneGraph->draw(exclude);
	skybox->draw(glm::mat4(glm::mat3(view)), projection);
}

void GameScene::customRender(glm::mat4 view, glm::mat4 projection, std::vector<GraphNode*> exclude) {
	sceneManager->getUboViewProjection()->inject(view, projection);
	for (int i = 0; i < updatableShaders.size(); i++) {
		updatableShaders[i]->setViewPosition(view[3]);
	}
	sceneGraph->draw(exclude);
	skybox->draw(glm::mat4(glm::mat3(view)), projection);
}

void GameScene::updatePoints() {
	sprintf_s(pointsText, "%d points", points);
}

void GameScene::pause() {
	pauseScene->setResumeCallback([this]() { unpause(); });
	paused = true;
	if (freeCam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void GameScene::unpause() {
	paused = false;
	if (freeCam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
