#include "ppDrawObject.h"

ppDrawObject::ppDrawObject() {

}

void ppDrawObject::draw()
{
	// Nothing to draw yet ...
	// Any derived class should implement this
	// This is here so we can call it
}

void ppDrawObject::draw(float x, float y, float w, float h)
{
	// Nothing to draw yet ...
	// Any derived class should implement this
	// This is here so we can call it
}

void ppDrawObject::setBlendMode(ofBlendMode blendmode) {
	_blendmode = blendmode;
}

ofBlendMode ppDrawObject::getBlendMode() {
	return _blendmode;
}

bool ppDrawObject::isInteractive() {
	return _isInteractive;
}

bool ppDrawObject::isAnimated() {
	return _isAnimated;
}

bool ppDrawObject::hasSound() {
	return _hasSound;
}

bool ppDrawObject::isFresh()
{
	return _isFresh;
}

void ppDrawObject::setFresh(bool fresh)
{
	_isFresh = fresh;
}

unsigned int ppDrawObject::getWidth()
{
	return _width;
}

unsigned int ppDrawObject::getHeight()
{
	return _height;
}

void ppDrawObject::setWidth(unsigned int width)
{
	_width = width;
}

void ppDrawObject::setHeight(unsigned int height)
{
	_height = height;
}

ofImageType ppDrawObject::getImageType()
{
	return _imageType;
}
