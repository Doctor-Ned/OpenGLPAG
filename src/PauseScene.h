#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "Scene.h"
#include <functional>
#include "TextRenderer.h"
#include "UiTextButton.h"
#include "UiColorPlane.h"

class PauseScene : public Scene {
public:
	PauseScene();
	void render() override;
	void update(double timeDelta) override;
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int butt, int action, int mods) override;
	void setResumeCallback(const std::function<void()>& resumeCallback);
protected:
	TextRenderer *textRenderer;
	UiTextButton *buttonResume, *buttonMenu;
	UiColorPlane *background;
	std::function<void()> resumeCallback = []() {};
};

#endif