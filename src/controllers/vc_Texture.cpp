#pragma once
#include "vc_Texture.h"
#include "ppTexture.h"

TextureController::TextureController()
	: ppVectorController()
{
	// Manual Mapping ...
	mapObjectTypes["Texture"] = createInstance<ppTexture>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("Textures");
}
