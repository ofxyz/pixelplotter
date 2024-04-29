#include "ofMain.h"
#include "imageFilter.h"

ImageFilter::ImageFilter()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	// TODO: Add blend mode for Image Filter Drawing
}


void ImageFilter::renderUpDownButtons()
{
	if (ImGui::Button("Move Up"))
	{
		moveUp = true;
		moveDown = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Move Down"))
	{
		moveUp = false;
		moveDown = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Duplicate")) {
		duplicate = true;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Visible", &bVisible)) {
		setFresh(true);
	}
}

bool ImageFilter::isFresh()
{
	return bFresh;
}

void ImageFilter::setFresh(bool fresh)
{
	bFresh = fresh;
}

bool ImageFilter::isAlive()
{
	return bAlive;
}

void ImageFilter::setAlive(bool alive)
{
	bAlive = alive;
}
