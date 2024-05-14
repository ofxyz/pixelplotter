#include "ofMain.h"
#include "imageFilter.h"

ImageFilter::ImageFilter()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	// TODO: Add blend mode for Image Filter Drawing
}

bool ImageFilter::isFresh()
{
	return bFresh;
}

void ImageFilter::setFresh(bool fresh)
{
	bFresh = fresh;
}

bool ImageFilter::isAlive()
{
	return bAlive;
}

void ImageFilter::setAlive(bool alive)
{
	bAlive = alive;
}
