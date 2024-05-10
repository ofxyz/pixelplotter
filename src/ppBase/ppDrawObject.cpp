#include "ppDrawObject.h"

ppDrawObject::ppDrawObject() {

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
