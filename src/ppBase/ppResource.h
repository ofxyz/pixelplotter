#pragma once
#include "ppBase.h"
#include "ofJson.h"
#include "string"

class ppResource : public virtual ppBase {
public:
	ppResource();

	void setName(std::string sName);
	std::string getName();
	bool hasSound();
	void setSound(bool hasSound);

	virtual void setup()  = 0;
	virtual bool update() = 0; // Returns freshness on change
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson & settings) = 0;
	virtual ofJson getSettings() = 0;

private:
	bool _hasSound = false;
	std::string _sName = "Untitled Resource";
};
