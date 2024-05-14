#include "ppDrawObject.h"

ppDrawObject::ppDrawObject():
	ppBase(){

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
	return _bHasSound;
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
