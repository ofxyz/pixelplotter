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

bool ppResource::isAlive()
{
	return _alive;
}

void ppResource::setAlive(bool alive)
{
	_alive = alive;
}

bool ppResource::isOpen()
{
	return _isOpen;
}

void ppResource::setOpen(bool isOpen)
{
	_isOpen = isOpen;
}
