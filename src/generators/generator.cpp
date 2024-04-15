#include "generator.h"

Generator::Generator(int _width /* = 100*/, int _height /* = 100*/)
{
	width  = _width;
	height = _height;
	c_bg = ofColor(255, 255, 255, 255);
	m_fbo.allocate(width, height);
}

bool Generator::isFresh()
{
	return bFresh;
}

void Generator::setFresh(bool fresh)
{
	bFresh = fresh;
}
