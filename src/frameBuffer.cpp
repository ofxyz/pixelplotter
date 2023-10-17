#include "ofMain.h"
#include "frameBuffer.h"

void FrameBuffer::setup(ofPixels& firstFrame)
{
	qFrames.clear();
	currentFrame.getPixelsRef() = firstFrame;
	currentFrame.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	currentFrame.update();
	bFresh = true;
}

void FrameBuffer::addFrame(ofPixels& freshPixels)
{
	qFrames.push_back(freshPixels);
}

void FrameBuffer::update()
{
	if (!bFresh && qFrames.size() > 0) {
		currentFrame.getPixelsRef() = qFrames[0];
		currentFrame.update();
		qFrames.pop_front();
		bFresh = true;
	}
}

ofImage& FrameBuffer::getFrame()
{
	bFresh = false;
	return currentFrame;
}

ofColor FrameBuffer::getColor(int x, int y)
{
	return currentFrame.getPixels().getColor(x, y);
}

ofColor FrameBuffer::getColor(int x, int y, int width, int height)
{
	return currentFrame.getPixels().getColor(round(ofMap(x, 0, width, 0, getWidth())), round(ofMap(y, 0, height, 0, getHeight())));
}

float FrameBuffer::getWidth()
{
	return currentFrame.getWidth();
}

float FrameBuffer::getHeight()
{
	return currentFrame.getHeight();
}

bool FrameBuffer::isFresh()
{
	return bFresh;
}
