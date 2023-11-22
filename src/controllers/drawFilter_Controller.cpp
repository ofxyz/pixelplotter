#pragma once
#include "ofx2d.h"

#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "ImGui_Widget_Tooltip.h"
#include "ImGui_Widget_InputTextString.h"

#include "drawFilter_Controller.h"
#include "drawFilter_Pixelate.h"
#include "drawFilter_Pixelate2.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"
#include "drawFilter_Mesh.h"
#include "drawFilter_Lumes.h"
#include <generator_Controller.h>

DrawFilterController::DrawFilterController()
	: Controller()
{
	// Manual Mapping ...
	v_objectNames = {
		"Lumes",
		"Mesh",
		"Noise",
		"Pixelate",
		"Pixelate2",
		"Rings"
	};

	mapObjectTypes["Lumes"]     = createInstance<Df_lumes>;
	mapObjectTypes["Mesh"]      = createInstance<Df_mesh>;
	mapObjectTypes["Noise"]     = createInstance<Df_noise>;
	mapObjectTypes["Pixelate"]  = createInstance<Df_pixelate>;
	mapObjectTypes["Pixelate2"] = createInstance<Df_pixelate2>;
	mapObjectTypes["Rings"]     = createInstance<Df_rings>;

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