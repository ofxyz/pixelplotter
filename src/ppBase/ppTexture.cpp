#include "ppTexture.h"
#include "ofApp.h"

ppTexture::ppTexture() {
	//allocate(getWidth(), getHeight(), GL_RGBA);
	setName("Untitled Texture");
	addProperty("Width", EPT_UINT, &_width);
	addProperty("Height", EPT_UINT, &_height);
	addProperty("Background", EPT_VEC4, &cBG);
	cBG = ofColor(255, 0, 0, 255);
	_tFbo.allocate(_width, _height, GL_RGBA);
	

}

void ppTexture::setup() {
	
}

bool ppTexture::update() {
	return false;
}

void ppTexture::renderImGuiSettings(bool bWrapped /* 1 */) {

	if (bWrapped) {
		ImGui::SetNextItemOpen(_bIsOpen, ImGuiCond_Once);
		if (ImGui::CollapsingHeader(getName().c_str(), &_bAlive)) {
			_bIsOpen = true;

			renderImGuiSettings(!bWrapped /* false */);
		}
		else {
			_bIsOpen = false;
		}
	}

	ImGui::Text("Dimensions"); ImGui::SameLine(60);
	const unsigned int dmin = 1;
	const unsigned int dmax = 1200;

	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("W ###wtexture" + getID(), ImGuiDataType_U32, &_width, 1, &dmin, &dmax)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("H ###htexture" + getID(), ImGuiDataType_U32, &_height, 1, &dmin, &dmax)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();
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
	_tFbo.begin();
	ofClear(255, 255, 255, 0);
	ofSetColor(255, 0, 0, 255);
	ofDrawRectangle(0, 0, _width, _height);
	_tFbo.end();
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
