#include "drawFilter_Rings.h"

// - Create smooth curves
// - Remove edge lines

ofxXmlSettings Df_rings::getSettings() {
	ofxXmlSettings settings;
	return settings;
}

void Df_rings::loadSettings(ofxXmlSettings settings) {
	return;
}

void Df_rings::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();
		ImGui::Checkbox("Visible", &visible);

		ImGui::PushItemWidth(100);
		ImGui::DragInt("Blur ##rings", &cvBlur, 1, 0, 500);
		ImGui::DragInt("Threshold ##rings", &cvThresh, 1, 0, 255);
		ImGui::DragInt("Ring Count ##rings", &cvSteps, 1, 1, 255);
		ImGui::DragFloat("Line Width ##rings", &lineWidth, 0.1f, 0, 50, "%.3f");

		ImGui::PopItemWidth();
	}
}

void Df_rings::draw(ofImage* input) {
	if (!visible) return;

	// -------------------------------- Update
	colorCvImage.allocate(input->getWidth(), input->getHeight());
	greyCvImage.allocate(input->getWidth(), input->getHeight());
	greyCvBlur.allocate(input->getWidth(), input->getHeight());
	greyCvThresh.allocate(input->getWidth(), input->getHeight());

	colorCvImage.setFromPixels(input->getPixelsRef());
	greyCvImage = colorCvImage; // Convert to Grey
	greyCvBlur = greyCvImage;
	greyCvBlur.blur(cvBlur);
	
	//---------------------------------
	
	ofPushStyle();
	ofSetColor(c_blob);
	ofNoFill();
	ofSetLineWidth(lineWidth);

	int i = 0;
	ofColor c(255, 255, 255);
	ofPolyline blobShape;
	//ofPolyline smoothShape;

	while (i < (250 - cvSteps)) {
		if (i > cvThresh)
			break;
		i += cvSteps;
		greyCvThresh = greyCvBlur;
		greyCvThresh.threshold(i);
		contourFinder.findContours(greyCvThresh, 5, (input->getWidth() * input->getHeight()) / 2, 25, true, true);
		//contourFinder.draw();
		for (int i = 0; i < contourFinder.blobs.size(); i++) {
			blobShape.clear();
			//c_blob.setHsb(c_blob.getHue() + 10, 255, 255);
			//ofSetColor(c_blob);
			blobShape.addVertices(contourFinder.blobs.at(i).pts);
			blobShape.close();
			blobShape.scale(drawScale, drawScale);
			//smoothShape = blobShape.getSmoothed(10, 0.5);
			//smoothShape.draw();
			blobShape.draw();
		}
	}

	ofPopStyle();
}
