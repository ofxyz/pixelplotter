#include "ppSurface.h"

ppSurface::ppSurface() {

}

void ppSurface::setName(std::string sName) {
	_sName = sName;
}

std::string ppSurface::getName() {
	return _sName;
}

void ppSurface::setup()
{

}

bool ppSurface::update()
{
	return isFresh();
}

void ppSurface::renderImGuiSettings(bool wrapped /*= 0*/)
{

}

void ppSurface::loadSettings(ofJson& settings)
{

}

ofJson ppSurface::getSettings()
{
	ofJson settings;
	return settings;
}
