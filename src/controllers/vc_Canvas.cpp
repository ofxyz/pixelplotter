#pragma once
#include "vc_Canvas.h"

CanvasController::CanvasController()
	: ppVectorController()
{
	// Manual Mapping ...
	mapObjectTypes["Canvas"] = createInstance<ppCanvas>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("Canvases");
}
