#include "ppBase.h"

unsigned int ppBase::m_uiIdCounter = 0;

ppBase::ppBase()
	: m_pBaseManager(std::shared_ptr<ppBaseManager>(nullptr))
{
	
	m_uiId = ++m_uiIdCounter;
}

unsigned int ppBase::getId() {
	return m_uiId;
}

std::vector<sProp>& ppBase::getProperties()
{
	return m_vProps;
}

void ppBase::addProperty(std::string name, propTypes type, void* data)
{
	sProp prop;
	prop.id   = ++m_uiIdCounter;
	prop.name = name;
	prop.type = type;
	prop.data = data;
	m_vProps.emplace_back(prop);
}

bool ppBase::isFresh()
{
	return m_isFresh;
}

void ppBase::setFresh(bool fresh)
{
	m_isFresh = fresh;
}

bool ppBase::isAlive()
{
	return m_isAlive;
}

void ppBase::setAlive(bool alive)
{
	m_isAlive = alive;
}
