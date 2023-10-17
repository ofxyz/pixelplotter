#pragma once

#include "imageFilter.h"

class If_tint : public ImageFilter {
public:
	If_tint() {
		name = "Tint";
	};

	virtual void apply(ofImage* img);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofJson settings);
	virtual ofJson getSettings();

private:
	ImVec4 c_Tint = ofColor(255, 255, 255, 255);
	bool bGrey = false;
};
