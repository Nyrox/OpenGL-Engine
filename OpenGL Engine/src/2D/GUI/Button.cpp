#include "Button.h"

Button::Button(uint32_t width, uint32_t height) : sprite(width, height) {

}

void Button::render(Shader& shader) {

	sprite.draw(shader);
}