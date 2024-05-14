#pragma once
#include "ppBase.h"
#include "ofJson.h"
#include "string"
#include "ppTexture.h"

class ppSurface : public ppBase {
public:
	ppSurface();

	void setName(std::string sName);
	std::string getName();

	void setup(ppTexture* texture);
	void assignTexture(ppTexture* texture);
	void setDimensions(float width, float height);

	bool update(); // Returns freshness on change
	void draw();
	void renderImGuiSettings(bool wrapped = 0);
	void loadSettings(ofJson & settings);
	ofJson getSettings();

protected:
	bool _bIsOpen = true;
	bool _bAlive = true;
	glm::vec2 _dimensions;
	std::string _sName = "Untitled";
	ppTexture* _texture = nullptr;
	ImVec4 cBG;
};