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
	virtual void renderImGuiSettings(bool wrapped = 0) = 0;
	virtual void loadSettings(ofJson & settings) = 0;
	virtual ofJson getSettings() = 0;

	bool isSelected();
	void setSelected(bool selected);

	bool isOpen();
	void setOpen(bool isOpen);

protected:
	bool _isSelected = false;
	// TODO REFACTOR
	bool _bDuplicate = false;
	bool _bAlive = true;
	bool _bHasSound = false;
	bool _bIsOpen = false;
	std::string _sName = "Untitled Resource";
};
