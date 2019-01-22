#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H
#include "Scene.h"
#include "MainMenuScene.h"
#include "UiSlider.h"

class SceneManager;

class OptionsScene : public Scene {
public:
	OptionsScene();
	void render() override;
	void update(double timeDelta) override;
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) override;
protected:
	void updateDifficulty(float diff);
	TextRenderer* textRenderer;
	glm::vec2 difficultyTextPos, difficultyPos, inverseTextPos;
	glm::vec3 difficultyColor;
	char* difficultyText;
	UiTextButton* buttonBackToMenu;
	UiSlider* sliderDifficulty;
	UiCheckbox *checkboxInverse;
	SceneManager *sceneManager;
};

#endif
