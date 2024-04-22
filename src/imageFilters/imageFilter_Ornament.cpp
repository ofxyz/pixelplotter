#include "imageFilter_Ornament.h"
#include "ofFbo.h"

ofJson If_ornament::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["hCount"] = hCount;
	settings["vCount"] = vCount;
	settings["bMirror"] = bMirror;
	
	return settings;
}


void If_ornament::onTileSizeChanged(int & t) {
}


void If_ornament::onWallpaperGroupChanged(int & t) {
}


void If_ornament::onAngleChanged(float & t) {
}

void If_ornament::loadSettings(ofJson& settings) {
	//name = settings.value("name", "duplicate");
	_isOpen = settings.value("_isOpen", _isOpen);
	hCount = settings.value("hCount", hCount);
	vCount = settings.value("vCount", vCount);
	bMirror = settings.value("bMirror", bMirror);
	return;
}

void If_ornament::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();
		//ImGui::PushItemWidth(60);

		if (ImGui::SliderInt("WPG ##ornament_TileSize", &wallpaperGroup, 9, 16)) {
			ornament.setWallpaperGroup(wallpaperGroup);
			setFresh(true);
		}

		// TODO: Division type of control?
		if (ImGui::DragInt("TileSize ##ornament_TileSize", &tileSize, 1, 1, 2000)) {
			ornament.setTileSize(tileSize);
			setFresh(true);
		}

		if (ImGui::SliderFloat("Angle ##ornament_Angle", &angle, -180, 180, "%.1f")) {
			ornament.setAngle(angle);
			setFresh(true);
		}

		//ImGui::PopItemWidth();
	} else {
		_isOpen = false;
	}
}

void If_ornament::apply(ofImage* img) {
	ofEnableArbTex();
	ornament.loadTexture(img->getTexture());
	ofDisableArbTex();
	ornament.update();

	ofFbo cfbo;
	cfbo.allocate(ornament.getTexture().getWidth(), ornament.getTexture().getHeight(), ornament.getTexture().getTextureData().glInternalFormat);
	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(0, 0, 0, 0));

	ornament.draw(0, 0);
	// Move to GUI
	//ornament.drawDebug(5, 5, 100, 100);
	
	cfbo.end();
	cfbo.readToPixels(img->getPixels());
	img->update();
	
	setFresh(false);
}