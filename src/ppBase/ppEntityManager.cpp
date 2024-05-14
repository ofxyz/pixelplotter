#pragma once

#include "ppEntityManager.h"

ppEntityManager::ppEntityManager()
{
	// 0
}

std::vector<std::shared_ptr<ppBase>>& ppEntityManager::getEntities(eEntityTypes type)
{
	static std::vector<std::shared_ptr<ppBase>> vEmpty;

	switch (type)
	{
	case EET_TEXTURE:
		return vTextures;
		break;
	case EET_SURFACE:
		return vSurfaces;
		break;
	default:
		return vEmpty;
		break;
	}
}

std::shared_ptr<ppBase>& ppEntityManager::createEntity(eEntityTypes type)
{
	return std::shared_ptr<ppBase>(nullptr);
}
