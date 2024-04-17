#include "imageFilter_Rotate.h"
#include "ofMain.h"
#include "ofApp.h"

ofJson If_rotate::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["bRotateCanvas"] = bRotateCanvas;
	settings["rCount"] = rCount;
	settings["vMirror"] = vMirror;
	settings["hMirror"] = hMirror;
	return settings;
}

void If_rotate::loadSettings(ofJson& settings) {
	//name = settings.value("name", "Mirror");
	_isOpen = settings.value("_isOpen", _isOpen);
	bRotateCanvas = settings.value("bRotateCanvas", bRotateCanvas);
	rCount  = settings.value("rCount", rCount);
	vMirror = settings.value("vMirror", vMirror);
	hMirror = settings.value("hMirror", hMirror);
	return;
}

void If_rotate::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();
		if (ImGui::SliderInt("Rotations", &rCount, 0, 3)) {
			setFresh(true);
		}
		if (ImGui::Checkbox("Rotate Canvas", &bRotateCanvas)) {
			setFresh(true);
		}
		if (ImGui::Checkbox("Flip Horizontal", &hMirror)) {
			setFresh(true);
		}
		if (ImGui::Checkbox("Flip Vertical", &vMirror)) {
			setFresh(true);
		}
	} else {
		_isOpen = false;
	}
}

void If_rotate::apply(ofImage* img) {
	img->rotate90(rCount);
	img->mirror(vMirror, hMirror);
	if (pixelplotter != nullptr && bRotateCanvas) {
		pixelplotter->plotCanvas.setDimensions(img->getWidth(), img->getHeight());
	}
	setFresh(false);
}
