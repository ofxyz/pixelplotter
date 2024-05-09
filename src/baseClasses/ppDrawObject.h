#pragma once
#include "ppResource.h"
#include "ofGraphicsConstants.h"

class ppDrawObject : public virtual ppResource {
public:
	ppDrawObject();

	virtual void draw() = 0;
	void setBlendMode(ofBlendMode blendmode);
	ofBlendMode getBlendMode();

	bool isInteractive();
	bool isAnimated();
	bool hasSound();

private:
	ofBlendMode _blendmode = OF_BLENDMODE_DISABLED;
	bool _isInteractive = false;
	bool _isAnimated    = false;
	bool _hasSound      = false;
};
