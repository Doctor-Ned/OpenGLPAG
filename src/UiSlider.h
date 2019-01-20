#ifndef UISLIDER_H
#define UISLIDER_H

#include "Headers.h"
#include "UiElement.h"

class UiSlider : public UiElement {
public:
	UiSlider(Shader *shader, char* textureIdle, char* textureHover, char* textureClicked, glm::vec2 position, glm::vec2 size, float lineThickness, glm::vec2 buttonSize
		, float value, float min, float max, bool center = true);
	void render() override;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
protected:
	void(*callback)(float) = nullptr;
};

#endif