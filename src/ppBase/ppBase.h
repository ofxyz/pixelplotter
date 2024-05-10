#pragma once

class ppBase {
public:
    ppBase();

    unsigned int getID();

private:
	static unsigned int uiID_Counter;
	unsigned int uiID;
};
