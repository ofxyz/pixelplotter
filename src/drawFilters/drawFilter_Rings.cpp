#include "drawFilter_Rings.h"
#include "ofApp.h"

// - Create smooth curves
// - Remove edge lines

ofJson Df_rings::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["cvThresh"] = cvThresh;
	settings["cvBlur"] = cvBlur;
	settings["cvSteps"] = cvSteps;
	settings["lineWidth"] = lineWidth;

	settings["colors"]["blob"]["r"] = c_blob.x;
	settings["colors"]["blob"]["g"] = c_blob.y;
	settings["colors"]["blob"]["b"] = c_blob.z;
	settings["colors"]["blob"]["a"] = c_blob.w;

	return settings;
}


void Df_rings::loadSettings(ofJson& settings) {
	try {
		//name = settings.value("name", "rings");
		_isOpen = settings.value("_isOpen", _isOpen);
		cvThresh = settings.value("cvThresh", cvThresh);
		cvBlur = settings.value("cvBlur", cvBlur);
		cvSteps = settings.value("cvSteps", cvSteps);
		lineWidth = settings.value("lineWidth", lineWidth);

		c_blob.x = settings.value("colors:blob:r", c_blob.x);
		c_blob.y = settings.value("colors:blob:g", c_blob.y);
		c_blob.z = settings.value("colors:blob:b", c_blob.z);
		c_blob.w = settings.value("colors:blob:a", c_blob.w);
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_rings";
	}
	return;
}

void Df_rings::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		ImGui::PushItemWidth(100);
		if (ImGui::DragInt("Blur ##rings", &cvBlur, 1, 0, 500)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("SFX ##blurRings", &sBlurMapTo, 0.1f, 1, 600, "%.3f")) {
			setFresh(true);
		}

		if (ImGui::DragInt("Threshold ##rings", &cvThresh, 1, 0, 255)) {
			setFresh(true);
		}
		if (ImGui::DragInt("Ring Count ##rings", &cvSteps, 1, 1, 255)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("SFX ##stepRings", &sStepMapTo, 0.1f, 1, 500, "%.3f")) {
			setFresh(true);
		}

		if (ImGui::DragFloat("Line Width ##rings", &lineWidth, 0.1f, 0.1, 50, "%.3f")) {
			setFresh(true);
		}

		ImGui::PopItemWidth();

		if (ImGui::ColorEdit4("Blobline ##rings", (float*)&c_blob, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}
	} else {
		_isOpen = false;
	}
}

void Df_rings::draw(ofImage* input, float width, float height) {
	setFresh(false);
	if (!bVisible) return;

	// -------------------------------- Update -> MOVE TO!
	colorCvImage.allocate(input->getWidth(), input->getHeight());
	greyCvImage.allocate(input->getWidth(), input->getHeight());
	greyCvBlur.allocate(input->getWidth(), input->getHeight());
	greyCvThresh.allocate(input->getWidth(), input->getHeight());

	colorCvImage.setFromPixels(input->getPixels());
	greyCvImage = colorCvImage; // Convert to Grey
	greyCvBlur = greyCvImage;
	//greyCvBlur.blur(cvBlur);
	int greySoundBlur = cvBlur - (ofMap(pixelplotter->soundManager.scaledVol, 0, 1, 0, sBlurMapTo));
	if (greySoundBlur > 0) {
		greyCvBlur.blur(greySoundBlur);
	}

	//---------------------------------

	ofPushStyle();
	ofSetColor((ofColor)c_blob);
	ofNoFill();
	ofSetLineWidth(lineWidth);

	int i = 0;
	ofColor c(c_blob);
	//c.setHsb(100, 100, 100);
	ofPolyline blobShape;
	//ofPolyline smoothShape;
	int sCvSteps = cvSteps;
	if (sStepMapTo > 1) {
		sCvSteps = cvSteps - (ofMap(pixelplotter->soundManager.scaledVol, 0, 1, 1, sStepMapTo));
		if (sCvSteps < 1) sCvSteps = 1;
		if (sCvSteps > 254) sCvSteps = 254;
	}


	while (i < (255 - sCvSteps)) {
		if (i > cvThresh)
			break;
		i += sCvSteps;
		greyCvThresh = greyCvBlur;
		greyCvThresh.threshold(i);
		contourFinder.findContours(greyCvThresh, 5, (input->getWidth() * input->getHeight()) / 2, 25, true, true);
		//contourFinder.draw(x,y,width,height);

		for (int i = 0; i < contourFinder.blobs.size(); i++) {
			blobShape.clear();
			//c.setHsb(c.getHue() + 10, 255, 255);
			float b = c.getBrightness() + 20;
			if (b > 255) b -= 255;
			c.setHsb(c.getHue(), c.getSaturation(), b);
			//ofSetColor(c);
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
