#include "ppTexture.h"
#include "ofApp.h"

ppTexture::ppTexture() {
	//allocate(getWidth(), getHeight(), GL_RGBA);
	setName("Untitled");
	addProperty("Name", EPT_STRING, &_sName);
	addProperty("Width", EPT_UINT, &_width);
	addProperty("Height", EPT_UINT, &_height);
	addProperty("Background", EPT_IMVEC4, &cBG);
	cBG = ImVec4(1, 0, 0, 1);
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

	ImGui::AlignTextToFramePadding();
	ImGui::InputText("Name", &_sName);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size"); ImGui::SameLine(60);

	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("W", ImGuiDataType_U32, &_width)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("H", ImGuiDataType_U32, &_height)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();

	if (ImGui::ColorEdit4("Background", (float*)&cBG, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}
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
	ofSetColor(ofColor(cBG));
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
