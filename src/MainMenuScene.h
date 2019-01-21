#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H
#include "Scene.h"
#include "TextRenderer.h"
#include "UiTextButton.h"

class MainMenuScene : public Scene {
public:
	MainMenuScene();
	void render() override;
	void update(double timeDelta) override;
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) override;
	void setOptionsCallback(std::function<void()> callback);
protected:
	TextRenderer* textRenderer;
	UiTextButton *buttonNewGame, *buttonOptions, *buttonQuit;
};

#endif
