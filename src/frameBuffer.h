#pragma once

#include "ofMain.h"

class FrameBuffer {
public:

	void setup(ofPixels firstFrame) {
		qFrames.clear();
		currentFrame.getPixelsRef() = firstFrame;
		currentFrame.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		currentFrame.update();
		bFresh = true;
	}

	void addFrame(ofPixels freshPixels) {
		qFrames.push_back(freshPixels);
	}

	void update() {
		if (!bFresh && qFrames.size() > 0) {
			currentFrame.getPixelsRef() = qFrames[0];
			currentFrame.update();
			qFrames.pop_front();
			bFresh = true;
		}
	}

	ofImage& getFrame() {
		bFresh = false;
		return currentFrame;
	}

	ofColor getColor(int x, int y, int width, int height ) {
		return currentFrame.getPixels().getColor( round(ofMap(x,0,width,0, getWidth())), round(ofMap(y, 0, height, 0, getHeight())) );
	}

	ofColor getColor(int x, int y) {
		return currentFrame.getPixels().getColor(x, y);
	}

	float getWidth() {
		return currentFrame.getWidth();
	}

	float getHeight() {
		return currentFrame.getHeight();
	}

	bool isFresh() {
		return bFresh;
	}

private:
	bool bFresh = false;
	std::deque<ofPixels> qFrames;
	ofImage currentFrame;
};
