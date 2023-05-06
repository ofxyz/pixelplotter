#pragma once

#include "ofMain.h"

class FrameBuffer {
public:

	void setup(ofPixels firstFrame) {
		currentFrame.getPixelsRef() = firstFrame;
		currentFrame.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		currentFrame.update();
		setFresh(true);
	}

	void addFrame(ofPixels freshPixels) {
		qFrames.push_back(freshPixels);
	}

	void update() {
		if (!bFresh && qFrames.size() > 0) {
			currentFrame.getPixelsRef() = qFrames[0];
			currentFrame.update();
			qFrames.pop_front();
			setFresh(true);
		}
	}

	ofImage& getFrame() {
		setFresh(false);
		return currentFrame;
	}

	float getWidth() {
		return currentFrame.getWidth();
	}

	float getHeight() {
		return currentFrame.getHeight();
	}

	void setFresh(bool fresh) {
		bFresh = fresh;
	}

	bool isFresh() {
		return bFresh;
	}

private:
	bool bFresh = false;
	std::deque<ofPixels> qFrames;
	ofImage currentFrame;
};
