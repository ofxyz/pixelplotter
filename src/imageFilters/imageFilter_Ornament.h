#pragma once

#include "imageFilter.h"
#include "Ornament.h"

class If_ornament : public ImageFilter {
public:
	If_ornament() {
		name = "Ornament";
		ornament.setup();
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	Ornament ornament;
	int tileSize = 150;
	int wallpaperGroup = 12;
	float angle = 0;
};