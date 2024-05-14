#include "ppSurface.h"

ppSurface::ppSurface():
	ppDrawObject(){

}

void ppSurface::setName(std::string sName) {
	_sName = sName;
}

std::string ppSurface::getName() {
	return _sName;
}

void ppSurface::setup(ppTexture* texture)
{
	assignTexture(texture);
	setDimensions(texture->getWidth(), texture->getHeight());
}

void ppSurface::setup()
{
  // 0
}

void ppSurface::assignTexture(ppTexture* texture)
{
	_texture = texture;
}

void ppSurface::setDimensions(float width, float height)
{
	_dimensions.x = width;
	_dimensions.y = height;
}

bool ppSurface::update()
{
	return isFresh();
}

void ppSurface::draw()
{
	if (_texture != nullptr) {
		_texture->draw();
	}
}

void ppSurface::draw(float x, float y, float w, float h)
{
	if (_texture != nullptr) {
		_texture->draw(x, y, w, h);
	}
}

void ppSurface::renderImGuiSettings(bool wrapped /*= 0*/)
{

	if (wrapped) {
		ImGui::SetNextItemOpen(_bIsOpen, ImGuiCond_Once);
		if (ImGui::CollapsingHeader(getName().c_str(), &_bAlive)) {
			_bIsOpen = true;

			renderImGuiSettings(!wrapped /* false */);
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
	if (ImGui::DragScalar("W", ImGuiDataType_U32, &_dimensions.x)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("H", ImGuiDataType_U32, &_dimensions.y)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();

	if (ImGui::ColorEdit4("Background", (float*)&cBG, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}
}

void ppSurface::loadSettings(ofJson& settings)
{

}

ofJson ppSurface::getSettings()
{
	ofJson settings;
	return settings;
}
