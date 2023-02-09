#include "drawFilters.h"

void Df_rings::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		// UI
	}
}

void Df_rings::draw(ofImage* input) {
	colorCvImage.setFromPixels(input->getPixelsRef());
	greyCvImage = colorCvImage; // Convert to Grey
	greyCvBlur = greyCvImage;
	greyCvBlur.blur(cvBlur);

	ofPushStyle();
	ofSetColor(ofColor(255, 0, 0));
	ofNoFill();
	ofSetLineWidth(4);

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
			c.setHsb(c.getHue() + 10, 255, 255);
			ofSetColor(c);
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
