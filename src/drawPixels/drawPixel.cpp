#include "drawPixel.h"

void DrawPixel::drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c)
{
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	ofDrawRectangle(-(w * 0.5) + offsetX, -(h * 0.5) + offsetY, w, h);
	ofPopStyle();
}

void DrawPixel::drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c)
{
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	//ofSetCircleResolution(4); Does not make sense for ciaro renderer.
	ofDrawEllipse(offsetX, offsetY, w, h);
	ofPopStyle();
}

void DrawPixel::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY)
{
	float cp1x = prevX + 2.0 / 3.0 * (cpx - prevX);
	float cp1y = prevY + 2.0 / 3.0 * (cpy - prevY);
	float cp2x = cp1x + (x - prevX) / 3.0;
	float cp2y = cp1y + (y - prevY) / 3.0;

	// finally call cubic Bezier curve function  
	ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
}

void DrawPixel::drawRoundedRect(float offsetX, float offsetY, float w, float h, ofColor c)
{
	// Use ofDrawRectRounded(p.x, p.y, 0.0, w, h, r,r,r,r) ?
	ofPushStyle();
	ofFill();
	ofSetColor(c);

	float x = -(w * 0.5) + offsetX;
	float y = -(h * 0.5) + offsetY;

	ofBeginShape();
	ofVertex(x + rounded, y);
	ofVertex(x + w - rounded, y);
	quadraticBezierVertex(x + w, y, x + w, y + rounded, x + w - rounded, y);
	ofVertex(x + w, y + h - rounded);
	quadraticBezierVertex(x + w, y + h, x + w - rounded, y + h, x + w, y + h - rounded);
	ofVertex(x + rounded, y + h);
	quadraticBezierVertex(x, y + h, x, y + h - rounded, x + rounded, y + h);
	ofVertex(x, y + rounded);
	quadraticBezierVertex(x, y, x + rounded, y, x, y + rounded);
	ofEndShape();

	ofPopStyle();
}

void DrawPixel::gui_setGrey()
{
	c_magentaRed = ofColor(108, 108, 108);
	c_cyanBlue = ofColor(139, 139, 139);
	c_yellowGreen = ofColor(239, 239, 239);
	c_black = ofColor(0, 0, 0);
	setFresh(true);
}

void DrawPixel::gui_setRGB()
{
	c_magentaRed = ofColor(255, 0, 0);
	c_cyanBlue = ofColor(0, 0, 255);
	c_yellowGreen = ofColor(0, 255, 0);
	c_black = ofColor(0, 0, 0);
	c_white = ofColor(255, 255, 255);
	setFresh(true);
}

void DrawPixel::gui_setCMYK()
{
	c_magentaRed = ofColor(236, 0, 140);
	c_cyanBlue = ofColor(0, 174, 239);
	c_yellowGreen = ofColor(255, 242, 0);
	c_black = ofColor(0, 0, 0);
	c_white = ofColor(255, 255, 255);
	setFresh(true);
}

void DrawPixel::gui_setRYB()
{
	c_magentaRed = ofColor(248, 11, 17);
	c_cyanBlue = ofColor(19, 57, 166);
	c_yellowGreen = ofColor(255, 230, 0);
	c_black = ofColor(0, 0, 0);
	c_white = ofColor(255, 255, 255);
	setFresh(true);
}

void DrawPixel::renderImGuiColourSettings()
{
	if (ImGui::CollapsingHeader("Colours ##drawFilter"))
	{
		/* TODO: Make it drag and Drop */
		if (ImGui::ColorEdit4("Cyan / Blue ##drawFilter", (float*)&c_cyanBlue, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}
		if (ImGui::ColorEdit4("Magenta / Red ##drawFilter", (float*)&c_magentaRed, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}
		if (ImGui::ColorEdit4("Yellow / Green ##drawFilter", (float*)&c_yellowGreen, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}
		if (ImGui::ColorEdit4("Key / Black ##drawFilter", (float*)&c_black, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}
		if (ImGui::ColorEdit4("Paper / White ##drawFilter", (float*)&c_white, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}

		if (ImGui::Button("Set Grey ##drawPixel"))
		{
			gui_setGrey();
		}
		ImGui::SameLine();
		if (ImGui::Button("Set RGB ##drawPixel"))
		{
			gui_setRGB();
		}
		ImGui::SameLine();
		if (ImGui::Button("Set CMYK ##drawPixel"))
		{
			gui_setCMYK();
		}
		ImGui::SameLine();
		if (ImGui::Button("Set RYB ##drawPixel"))
		{
			gui_setRYB();
		}
	} // End Colors
}

bool DrawPixel::isFresh()
{
	return bFresh;
}

void DrawPixel::setFresh(bool fresh)
{
	bFresh = fresh;
}
