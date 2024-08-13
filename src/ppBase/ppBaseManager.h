#pragma once

#include "ppBase.h"
#include <memory>

/*

   TODO: Create a place for docs, this needs to move?
   --------------------

   Texture
   - FBO

   Surface
   - Single node
   - Reference multiple textures

*/

typedef enum eBaseTypes {
	EET_TEXTURE = 1,
	EET_SURFACE,
	EET_COUNT
} eBaseTypes;

class ppBaseManager : public std::enable_shared_from_this<ppBaseManager> {
public:
	ppBaseManager();

	std::shared_ptr<ppBaseManager> getEntityManager();

	std::vector<std::shared_ptr<ppBase>>& getEntities(eBaseTypes type);
	std::shared_ptr<ppBase>& createEntity(eBaseTypes type);

private:
	std::vector<std::shared_ptr<ppBase>> vTextures;
	std::vector<std::shared_ptr<ppBase>> vSurfaces;
};
