#include "imageFilter.h"

bool ImageFilter::isFresh()
{
	return bFresh;
}

void ImageFilter::setFresh(bool fresh)
{
	bFresh = fresh;
}
