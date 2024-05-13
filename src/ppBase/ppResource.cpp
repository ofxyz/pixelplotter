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
	return _bHasSound;
}

void ppResource::setSound(bool hasSound) {
	_bHasSound = hasSound;
}

bool ppResource::isAlive()
{
	return _bAlive;
}

void ppResource::setAlive(bool alive)
{
	_bAlive = alive;
}

bool ppResource::isSelected()
{
	return _isSelected;
}

void ppResource::setSelected(bool selected)
{
	_isSelected = selected;
}

bool ppResource::isOpen()
{
	return _bIsOpen;
}

void ppResource::setOpen(bool isOpen)
{
	_bIsOpen = isOpen;
}
