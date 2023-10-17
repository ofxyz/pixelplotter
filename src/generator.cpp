#include "generator.h"

Generator::Generator()
{
	c_bg = ofColor(255, 255, 255, 255);
}

bool Generator::isFresh()
{
	return bFresh;
}

void Generator::setFresh(bool fresh)
{
	bFresh = fresh;
}
