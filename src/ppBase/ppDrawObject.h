#pragma once
#include "ppResource.h"
#include "ofGraphicsConstants.h"

class ppDrawObject : public virtual ppResource {
public:
	ppDrawObject();

	virtual void draw();
	virtual void draw(float x, float y, float w, float h);

	void setBlendMode(ofBlendMode blendmode);
	ofBlendMode getBlendMode();

	bool isInteractive();
	bool isAnimated();
	bool hasSound();

	bool isFresh();
	void setFresh(bool fresh);

	unsigned int getWidth();
	unsigned int getHeight();
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);

	ofImageType getImageType();

protected:
	unsigned int _width = 800;
	unsigned int _height = 600;
	ofImageType _imageType = OF_IMAGE_COLOR_ALPHA;
	GLint minFilter = GL_NEAREST;
	GLint magFilter = GL_NEAREST;
	ofBlendMode _blendmode = OF_BLENDMODE_DISABLED;

private:
	bool _isInteractive = false;
	bool _isAnimated = false;
	bool _bHasSound = false;
	bool _isFresh = false;
};
