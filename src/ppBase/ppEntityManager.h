#pragma once

#include "ppBase.h"
#include <memory>

typedef enum eEntityTypes {
	EET_TEXTURE = 1,
	EET_SURFACE,
	EET_COUNT
} eEntityTypes;

class ppEntityManager : public std::enable_shared_from_this<ppEntityManager> {
public:
	ppEntityManager();

	std::shared_ptr<ppEntityManager> getEntityManager();

	std::vector<std::shared_ptr<ppBase>>& getEntities(eEntityTypes type);
	std::shared_ptr<ppBase>& createEntity(eEntityTypes type);

	std::vector<std::shared_ptr<ppBase>> vTextures;
	std::vector<std::shared_ptr<ppBase>> vSurfaces;

private:
	std::shared_ptr<ppEntityManager> m_pThis;
};
