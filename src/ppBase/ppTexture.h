#pragma once
#include "ppDrawObject.h"
#include "ofFbo.h"
#include "ofImage.h"
#include "ImGui_General.h"

class ppTexture : public virtual ppDrawObject {
public:
	ppTexture();

	void setup() override;
	bool update() override;
	void renderImGuiSettings(bool bWrapped = 1) override;

	ofJson getSettings() override;
	void loadSettings(ofJson & settings) override;

	void draw() override;
	void draw(float x, float y, float w, float h) override;

	void draw(float x, float y);
	void draw(glm::vec2 position);
	void draw(glm::vec2 position, glm::vec2 dimensions);

	// GLint internalformat ///< GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc
	void allocate(unsigned int width, unsigned int height, GLint internalformat = GL_RGBA);
	void resize(unsigned int width, unsigned int height);

protected:
	ofFbo _tFbo;
	ofTexture _texture;
	ImVec4 cBG;
};
