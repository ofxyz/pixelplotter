#include "generator.h"

Generator::Generator(int _width /* = 100*/, int _height /* = 100*/)
{
	pixelplotter = (ofApp *)ofGetAppPtr();
	width  = _width;
	height = _height;
	c_bg = ofColor(255, 255, 255, 255);
	m_fbo.allocate(width, height);
}

void Generator::renderUpDownButtons() {
	if (ImGui::Button("Move Up")) {
		moveUp = true;
		moveDown = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Move Down")) {
		moveUp = false;
		moveDown = true;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Visible", &bVisible)) {
		setFresh(true);
	}
}

bool Generator::isFresh()
{
	return bFresh;
}

void Generator::setFresh(bool fresh)
{
	bFresh = fresh;
}

bool Generator::isAlive() {
	return bAlive;
}

void Generator::setAlive(bool alive) {
	bAlive = alive;
}
