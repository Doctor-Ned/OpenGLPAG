#include "UiSlider.h"

UiSlider::UiSlider(Shader* shader, char* textureIdle, char* textureHover, char* textureClicked, glm::vec2 position,
	glm::vec2 size, float lineThickness, glm::vec2 buttonSize, float value, float min, float max, bool center) : UiElement(shader, nullptr, position, size, center) {
	
}

void UiSlider::render() {}
void UiSlider::mouse_callback(GLFWwindow* window, double xpos, double ypos) {}
void UiSlider::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {}
