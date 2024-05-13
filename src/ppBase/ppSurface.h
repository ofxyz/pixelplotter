#pragma once
#include "ppBase.h"
#include "ofJson.h"
#include "string"

class ppSurface : public virtual ppBase {
public:
	ppSurface();

	void setName(std::string sName);
	std::string getName();

	void setup();
	bool update(); // Returns freshness on change
	void renderImGuiSettings(bool wrapped = 0);
	void loadSettings(ofJson & settings);
	ofJson getSettings();

protected:
	std::string _sName = "Untitled";
};
