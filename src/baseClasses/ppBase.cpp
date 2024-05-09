#include "ppBase.h"

unsigned int ppBase::uiID_Counter = 0;

ppBase::ppBase() {
	uiID = ++uiID_Counter;
}

unsigned int ppBase::getID() {
	return uiID;
}
