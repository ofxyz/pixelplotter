#include "ofMain.h"
#include "ofxImGui.h"
#include "drawFilter.h"

DrawFilter::DrawFilter()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	v_BlendModes.insert(v_BlendModes.end(), { "OF_BLENDMODE_DISABLED", "OF_BLENDMODE_ALPHA", "OF_BLENDMODE_ADD", "OF_BLENDMODE_SUBTRACT", "OF_BLENDMODE_MULTIPLY", "OF_BLENDMODE_SCREEN" });
}

void DrawFilter::renderUpDownButtons()
{
	if (ImGui::Button("Duplicate")) {
		duplicate = true;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Visible", &bVisible)) {
		setFresh(true);
	}
}

void DrawFilter::renderImGuiColourSettings(bool colors, bool mask)
{
	if (colors || mask) {
		if (ImGui::CollapsingHeader("Colours ##drawFilter"))
		{
			if (colors) {
				// TODO: REMOVE
			}
			if (mask) {
				if (ImGui::ColorEdit4("Mask ##drawFilter", (float*)&c_mask, ImGuiColorEditFlags_NoInputs)) {
					setFresh(true);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Use Mask ##drawFilter", &useMask)) {
					setFresh(true);
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(60);
				if (ImGui::DragInt("Margin ##drawFilter", &maskMargin, 1, 0, 255)) {
					setFresh(true);
				}
				ImGui::PopItemWidth();
			}

		} // End Colors
	}
}

void DrawFilter::setBlendMode()
{
	// TO DO: Make map for this and have it part of ofUtils or something
	// We can save BlendModeIndex but this object should not set it like this
	// Needs to be called like this: ofEnableBlendMode(BLENDMODES[BlendModeIndex]);
	
	if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_ALPHA") {
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	}
	else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_ADD") {
		ofEnableBlendMode(OF_BLENDMODE_ADD);
	}
	else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_SUBTRACT") {
		ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	}
	else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_MULTIPLY") {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	}
	else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_SCREEN") {
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	}
	else {
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	}
}

bool DrawFilter::isFresh()
{
	return bFresh;
}

void DrawFilter::setFresh(bool fresh)
{
	bFresh = fresh;
}

bool DrawFilter::isAlive()
{
	return bAlive;
}

void DrawFilter::setAlive(bool alive)
{
	bAlive = alive;
}
