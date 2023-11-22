#pragma once
#include "drawFilter_Controller.h"
#include "drawFilter_Pixelate.h"
#include "drawFilter_Pixelate2.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"
#include "drawFilter_Mesh.h"
#include "drawFilter_Lumes.h"

DrawFilterController::DrawFilterController()
	: Controller()
{
	// Manual Mapping ...
	
	mapObjectTypes["Lumes"]     = createInstance<Df_lumes>;
	mapObjectTypes["Mesh"]      = createInstance<Df_mesh>;
	mapObjectTypes["Noise"]     = createInstance<Df_noise>;
	mapObjectTypes["Pixelate"]  = createInstance<Df_pixelate>;
	mapObjectTypes["Pixelate2"] = createInstance<Df_pixelate2>;
	mapObjectTypes["Rings"]     = createInstance<Df_rings>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("DrawFilter");
}

void DrawFilterController::draw(ofImage* img, int width, int height)
{
	for (const auto& filter : v_Objects) {
		// TODO: update one filter per frame to keep things speedy?
		// Each filter draws to it's own fbo and are drawn here?
		// filter->update(img); filter->update(settings)
		filter->draw(img, width, height);
	}
	setFresh(false);
}