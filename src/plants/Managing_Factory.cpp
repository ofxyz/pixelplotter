#include "Managing_Factory.h"
#include "ofApp.h"

Managing_Factory::Managing_Factory()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
}

template <typename t> void Managing_Factory::move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
	if (oldIndex > newIndex)
		std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
	else
		std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

bool Managing_Factory::isFresh()
{
	return _bFresh;
}

void Managing_Factory::setFresh(bool fresh)
{
	_bFresh = fresh;
}
