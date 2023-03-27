#pragma once

#include "ofMain.h"

class FrameBuffer {
public:

	void setup(ofPixels firstFrame) {
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
	};

	void setFresh() {
		// Trigger a re-render without re-loading
		bFresh = true;
	};

	bool isFresh() {
		return bFresh;
	};

	ofImage& getFrame() {
		bFresh = false;
		return currentFrame;
	};

	float getWidth() {
		return currentFrame.getWidth();
	};

	float getHeight() {
		return currentFrame.getHeight();
	};

private:
	bool bFresh = false;
	std::deque<ofPixels> qFrames;
	ofImage currentFrame;
};
