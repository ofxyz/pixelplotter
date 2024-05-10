#pragma once

class FrameBuffer {
public:

	void setup(ofPixels& firstFrame);

	void addFrame(ofPixels& freshPixels);

	void update();

	ofImage& getFrame();

	ofColor getColor(int x, int y, int width, int height);

	ofColor getColor(int x, int y);

	float getWidth();

	float getHeight();

	bool isFresh();

private:
	bool bFresh = false;
	std::deque<ofPixels> qFrames;
	ofImage currentFrame;
};
