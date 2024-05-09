#include "ppResource.h"

ppResource::ppResource() {

}

void ppResource::setName(std::string sName) {
	_sName = sName;
}

std::string ppResource::getName() {
	return _sName;
}

bool ppResource::hasSound() {
	return _hasSound;
}

void ppResource::setSound(bool hasSound) {
	_hasSound = hasSound;
}
