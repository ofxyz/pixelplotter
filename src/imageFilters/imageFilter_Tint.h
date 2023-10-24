#pragma once

#include "imageFilter.h"

class If_tint : public ImageFilter {
public:
	If_tint() {
		name = "Tint";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

private:
	ImVec4 c_Tint = ofColor(255, 255, 255, 255);
	bool bGrey = false;
};
