#include "OptionsScene.h"
#include "SceneManager.h"

OptionsScene::OptionsScene() {
	sceneManager = &SceneManager::getInstance();
	textRenderer = sceneManager->getTextRenderer();
	glm::vec2 nextPos = difficultyTextPos = glm::vec2(WINDOW_CENTER_X, WINDOW_HEIGHT * 0.33f);
	nextPos.y += 45.0f;
	sliderDifficulty = new UiSlider(sceneManager->getUiTextureShader(), BUTTON_IDLE, BUTTON_HOVER,
		BUTTON_CLICKED, nextPos, BUTTON_STANDARD_SIZE
		, sceneManager->getUiColorShader(), BUTTON_STANDARD_SIZE.y / 4.0f,
		glm::vec2(BUTTON_STANDARD_SIZE.y * 0.75f, BUTTON_STANDARD_SIZE.y * 0.75f),
		sceneManager->difficulty, 0.0f, 4.0f);
	sliderDifficulty->setCallback([this](float diff) { updateDifficulty(diff); });
	updateDifficulty(sceneManager->difficulty);
	nextPos.y += BUTTON_STANDARD_SIZE.y - 15.0f;
	difficultyPos = nextPos;
	nextPos.y += BUTTON_STANDARD_SIZE.y + 5.0f;
	glm::vec2 checkboxPos = nextPos;
	checkboxPos.x -= 170.0f;
	checkboxInverse = new UiCheckbox(sceneManager->getUiTextureShader(), BUTTON_IDLE, BUTTON_HOVER, BUTTON_CLICKED, BUTTON_TICK_IDLE, BUTTON_TICK_HOVER, BUTTON_TICK_CLICKED,
		checkboxPos, CHECKBOX_STANDARD_SIZE, sceneManager->inverseEnabled);
	checkboxInverse->setCheckboxCallback([this](bool enabled) {
		sceneManager->inverseEnabled = enabled;
	});
	inverseTextPos = checkboxPos;
	inverseTextPos.x += CHECKBOX_STANDARD_SIZE.x;
	inverseTextPos.y -= 20.0f;
	nextPos.y += CHECKBOX_STANDARD_SIZE.y + 20.0f;
	buttonBackToMenu = new UiTextButton(sceneManager->getUiTextureShader(), BUTTON_LONG_IDLE,
		BUTTON_LONG_HOVER, BUTTON_LONG_CLICKED, nextPos
		, BUTTON_STANDARD_SIZE, sceneManager->getTextRenderer(),
		"Back to menu");
	buttonBackToMenu->setButtonCallback([this]() { sceneManager->backToMenu(); });
}

void OptionsScene::render() {
	textRenderer->renderText("OPTIONS", WINDOW_CENTER_X, WINDOW_CENTER_Y / 6.0f, 2.0f, true);
	textRenderer->renderText("DIFFICULTY", difficultyTextPos.x, difficultyTextPos.y, 0.75f, true);
	textRenderer->renderText(difficultyText, difficultyPos.x, difficultyPos.y, 1.5f, true, difficultyColor);
	textRenderer->renderText("INVERSE COLORS", inverseTextPos.x, inverseTextPos.y, 1.5f, false);
	checkboxInverse->render();
	buttonBackToMenu->render();
	sliderDifficulty->render();
}

void OptionsScene::update(double timeDelta) {}

void OptionsScene::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}

void OptionsScene::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	buttonBackToMenu->mouse_callback(window, xpos, ypos);
	sliderDifficulty->mouse_callback(window, xpos, ypos);
	checkboxInverse->mouse_callback(window, xpos, ypos);
}

void OptionsScene::mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) {
	buttonBackToMenu->mouse_button_callback(window, butt, action, mods);
	sliderDifficulty->mouse_button_callback(window, butt, action, mods);
	checkboxInverse->mouse_button_callback(window, butt, action, mods);
}

void OptionsScene::updateDifficulty(float diff) {
	sceneManager->difficulty = diff;
	if (diff <= 1.0f) {
		difficultyText = "Easy";
	} else if (diff <= 2.0f) {
		difficultyText = "Medium";
	} else if (diff <= 3.0f) {
		difficultyText = "Hard";
	} else {
		difficultyText = "Extreme";
	}
	difficultyColor = glm::vec3(remap(diff, 0.0f, 4.0f, 0.0f, 1.0f), remap(diff, 0.0f, 4.0f, 0.0f, 1.0f, true), 0.0f);
}
