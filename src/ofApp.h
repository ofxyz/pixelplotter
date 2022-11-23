#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void drawPixel_style000(float w, float h, ofColor c);
		void drawPixel_style001(float w, float h, ofColor c);
		void drawPixel_style002(float w, float h, ofColor c);
		void drawPixel_style003(float w, float h, ofColor c);
		void drawPixel_style004(float w, float h, ofColor c);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofImage img;
		ofPixels pixels;
		ofMesh mesh;
		ofColor penColor, paperColor;

		string img_name, img_ext;

		float imgRatio;
		int tilesX, tilesY, tileSize;

		bool drawImage, showImage, isLandscape;
		bool oneShot;
	
};
