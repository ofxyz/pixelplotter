#pragma once
#include "ppResource.h"
#include "ofGraphicsConstants.h"

//! A drawObject is any object that can be drawn to the screen
//! Meaning it will have a draw() function

class ppDrawObject : public virtual ppResource {
public:
	ppDrawObject();

	virtual void draw() = 0;
	virtual void draw(float x, float y, float w, float h) = 0;

	void setBlendMode(ofBlendMode blendmode);
	ofBlendMode getBlendMode();

	bool isInteractive();
	bool isAnimated();
	bool hasSound();

	unsigned int getWidth();
	unsigned int getHeight();
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);

	GLint getImageType();

protected:
	unsigned int _width = 800;
	unsigned int _height = 600;
	GLint _internalformat = GL_RGBA;
	GLint _minFilter = GL_NEAREST;
	GLint _magFilter = GL_NEAREST;
	ofBlendMode _blendmode = OF_BLENDMODE_DISABLED;

private:
	bool _isInteractive = false;
	bool _isAnimated = false;
	bool _bHasSound = false;
};
