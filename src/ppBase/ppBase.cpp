#include "ppBase.h"

unsigned int ppBase::uiID_Counter = 0;

ppBase::ppBase() {
	uiID = ++uiID_Counter;
}

unsigned int ppBase::getID() {
	return uiID;
}

std::vector<sProp>& ppBase::getProperties()
{
	return vProps;
}

void ppBase::addProperty(std::string name, pTypes type, void* pdata)
{
	sProp prop;
	prop.ID = ++uiID_Counter;
	prop.name = name;
	prop.type = type;
	prop.pData = pdata;
	vProps.emplace_back(prop);

}

bool ppBase::isFresh()
{
	return _isFresh;
}

void ppBase::setFresh(bool fresh)
{
	_isFresh = fresh;
}
