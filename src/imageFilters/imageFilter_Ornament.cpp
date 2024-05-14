#include "imageFilter_Ornament.h"
#include "ofFbo.h"

ofJson If_ornament::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["tileSize"] = tileSize;
	settings["wallpaperGroup"] = wallpaperGroup;
	settings["angle"] = angle;
	
	return settings;
}

void If_ornament::loadSettings(ofJson & settings) {
	//name = settings.value("name", "duplicate");
	_isOpen = settings.value("_isOpen", _isOpen);
	tileSize = settings.value("tileSize", tileSize);
	ornament.setTileSize(tileSize);
	wallpaperGroup = settings.value("wallpaperGroup", wallpaperGroup);
	ornament.setWallpaperGroup(wallpaperGroup);
	angle = settings.value("angle", angle);
	ornament.setAngle(angle);
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
		// tileSize should be devision of source width. Might need an offset control a well
		if (ImGui::DragInt("TileSize ##ornament_TileSize", &tileSize, 1, 1, 2000)) {
			ornament.setTileSize(tileSize);
			setFresh(true);
		}

		if (ImGui::DragFloat("Angle ##ornament_Angle", &angle, 1, -180, 180, "%.3f")) {
			ornament.setAngle(angle);
			setFresh(true);
		}

		//ImGui::PopItemWidth();

		// Move to GUI
		//ornament.drawDebug(5, 5, 100, 100);

	} else {
		_isOpen = false;
	}
}

void If_ornament::apply(ofImage* img) {
	if (!bVisible) return;
	ofEnableArbTex();
	ornament.loadTexture(img->getTexture());
	ornament.update();
	ofDisableArbTex();

	ofFbo cfbo;
	cfbo.allocate(img->getWidth(), img->getHeight(), img->getTexture().getTextureData().glInternalFormat);
	cfbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(0, 0, 0, 0));

	ornament.draw(0, 0);

	cfbo.end();

	cfbo.readToPixels(img->getPixels());
	img->update();
	
	setFresh(false);
}
