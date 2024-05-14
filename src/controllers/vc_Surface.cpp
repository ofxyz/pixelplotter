#pragma once
#include "vc_Surface.h"

SurfaceController::SurfaceController()
	: ppVectorController()
{
	// Manual Mapping
	mapObjectTypes["Surface"] = createInstance<ppSurface>;

	for (auto s : mapObjectTypes) {
		v_objectNames.push_back(s.first);
	}

	generateMenuNames("Surfaces");
}
