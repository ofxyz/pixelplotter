#pragma once
#include "ppTexture.h"

class ppCanvas : public virtual ppTexture {
public:
	ppCanvas();

	void draw(float x, float y, float w, float h) override;
	bool update() override;
	void renderImGuiSettings(bool bWrapped = 1) override;

	ofJson getSettings() override;
	void loadSettings(ofJson & settings) override;
};
