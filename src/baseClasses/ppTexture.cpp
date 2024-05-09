#include "ppTexture.h"

ppTexture::ppTexture() {
	allocate(_width, _height, _imageType);
}

void ppTexture::setup() {
}

bool ppTexture::update() {
	return false;
}

void ppTexture::renderImGuiSettings() {
}

ofJson ppTexture::getSettings() {
	ofJson settings;
	return settings;
}

void ppTexture::loadSettings(ofJson & settings) {
}

void ppTexture::draw() {
	draw(0, 0, _width, _height);
}

void ppTexture::draw(float x, float y, float w, float h) {
	_tFbo.draw(x, y, w, h);
}

void ppTexture::draw(float x, float y) {
	draw(x, y, _width, _height);
}

void ppTexture::draw(glm::vec2 pos) {
	draw(pos.x, pos.y, _width, _height);
}

void ppTexture::draw(glm::vec2 pos, glm::vec2 dim) {
	draw(pos.x, pos.y, dim.x, dim.y);
}

void ppTexture::allocate(unsigned int width, unsigned int height, ofImageType imageType /*= OF_IMAGE_COLOR_ALPHA*/) {
	_width = width;
	_height = height;
	_imageType = imageType;
	_tFbo.allocate(_width, _height, imageType);
	_tFbo.getTexture().setTextureMinMagFilter(minFilter, magFilter);
}

void ppTexture::resize(unsigned int width, unsigned int height) {
	allocate(width, height, _imageType);
}
