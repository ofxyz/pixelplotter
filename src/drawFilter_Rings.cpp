#include "drawFilter_Rings.h"

// - Create smooth curves
// - Remove edge lines

ofxXmlSettings Df_rings::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("cvThresh", cvThresh);
	settings.setValue("cvBlur", cvBlur);
	settings.setValue("cvSteps", cvSteps);
	settings.setValue("lineWidth", lineWidth);

	settings.setValue("colors:blob:r", c_blob.x);
	settings.setValue("colors:blob:g", c_blob.y);
	settings.setValue("colors:blob:b", c_blob.z);
	settings.setValue("colors:blob:a", c_blob.w);

	return settings;
}


void Df_rings::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", "rings");
	cvThresh = settings.getValue("cvThresh", cvThresh);
	cvBlur = settings.getValue("cvBlur", cvBlur);
	cvSteps = settings.getValue("cvSteps", cvSteps);
	lineWidth = settings.getValue("lineWidth", lineWidth);

	c_blob.x = settings.getValue("colors:blob:r", c_blob.x);
	c_blob.y = settings.getValue("colors:blob:g", c_blob.y);
	c_blob.z = settings.getValue("colors:blob:b", c_blob.z);
	c_blob.w = settings.getValue("colors:blob:a", c_blob.w);

	return;
}

void Df_rings::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		ImGui::PushItemWidth(100);
		if (ImGui::DragInt("Blur ##rings", &cvBlur, 1, 0, 500)) {
			bFresh = true;
		}
		if (ImGui::DragInt("Threshold ##rings", &cvThresh, 1, 0, 255)) {
			bFresh = true;
		}
		if (ImGui::DragInt("Ring Count ##rings", &cvSteps, 1, 1, 255)) {
			bFresh = true;
		}
		if (ImGui::DragFloat("Line Width ##rings", &lineWidth, 0.1f, 0, 50, "%.3f")) {
			bFresh = true;
		}
		ImGui::PopItemWidth();
	}
}

void Df_rings::draw(ofImage* input, float width, float height, float x, float y) {
	bFresh = false;
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
		//contourFinder.draw(x,y,width,height);

		for (int i = 0; i < contourFinder.blobs.size(); i++) {
			blobShape.clear();
			//c_blob.setHsb(c_blob.getHue() + 10, 255, 255);
			//ofSetColor(c_blob);
			blobShape.addVertices(contourFinder.blobs.at(i).pts);
			blobShape.close();
			blobShape.scale(width / input->getWidth(), height / input->getHeight());
			//smoothShape = blobShape.getSmoothed(10, 0.5);
			//smoothShape.draw();
			blobShape.draw();
		}
	}

	ofPopStyle();
}
