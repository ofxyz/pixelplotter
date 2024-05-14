#include "ppBase.h"

unsigned int ppBase::uiID_Counter = 0;

ppBase::ppBase()
	: m_pEntityManager(std::shared_ptr<ppEntityManager>(nullptr))
{
	
	uiID = ++uiID_Counter;
}

unsigned int ppBase::getID() {
	return uiID;
}

std::vector<sProp>& ppBase::getProperties()
{
	return vProps;
}

void ppBase::addProperty(std::string name, propTypes type, void* data)
{
	sProp prop;
	prop.id   = ++uiID_Counter;
	prop.name = name;
	prop.type = type;
	prop.data = data;
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
