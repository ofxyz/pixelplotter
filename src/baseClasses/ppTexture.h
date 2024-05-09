#pragma once
#include "ppDrawObject.h"
#include "ofFbo.h"
#include "ofImage.h"

class ppTexture : public virtual ppDrawObject {
public:
	ppTexture();

	void setup() override;
	bool update() override;
	void renderImGuiSettings() override;
	ofJson getSettings() override;
	void loadSettings(ofJson & settings) override;
	void draw() override;

	void draw(float x, float y);
	void draw(float x, float y, float w, float h);
	void draw(glm::vec2 position);
	void draw(glm::vec2 position, glm::vec2 dimensions);

	void allocate(unsigned int width, unsigned int height, ofImageType imageType = OF_IMAGE_COLOR_ALPHA);
	void resize(unsigned int width, unsigned int height);

private:
	unsigned int _width  = 800;
	unsigned int _height = 600;
	ofImageType _imageType = OF_IMAGE_COLOR_ALPHA;
	GLint minFilter = GL_NEAREST;
	GLint magFilter = GL_NEAREST;
	ofFbo _tFbo;
	ofTexture _texture;
};
