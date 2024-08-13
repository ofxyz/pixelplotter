#pragma once

#include "ppBaseManager.h"


ppBaseManager::ppBaseManager()
{
	// 0
}

std::shared_ptr<ppBaseManager> ppBaseManager::getEntityManager()
{
	return shared_from_this();
}

std::vector<std::shared_ptr<ppBase>>& ppBaseManager::getEntities(eBaseTypes type)
{
	static std::vector<std::shared_ptr<ppBase>> vReturnEmpty;

	switch (type)
	{
	case EET_TEXTURE:
		return vTextures;
		break;
	case EET_SURFACE:
		return vSurfaces;
		break;
	default:
		return vReturnEmpty;
		break;
	}
}

std::shared_ptr<ppBase>& ppBaseManager::createEntity(eBaseTypes type)
{
	
	auto pNewObject = std::shared_ptr<ppBase>(nullptr);
	if (pNewObject)
	{
		pNewObject->SetEntityManager(getEntityManager());
	}

	switch (type)
	{
	case EET_TEXTURE:
		
		break;
	case EET_SURFACE:
		
		break;
	default:
		
		break;
	}
	return pNewObject;
}
