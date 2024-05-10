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

	bool isAlive();
	void setAlive(bool alive);

	bool isOpen();
	void setOpen(bool isOpen);

	// TODO Refactor ...
	bool duplicate = false;
	bool _alive = true;

private:
	bool _hasSound = false;
	bool _isOpen = false;
	std::string _sName = "Untitled Resource";
};
