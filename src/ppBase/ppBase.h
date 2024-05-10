#pragma once

#include <string>
#include <vector>

typedef enum pTypes {
	EPT_BOOL,
	EPT_INT,
	EPT_UINT,
	EPT_FLOAT,
	EPT_DOUBLE,
	EPT_STRING
};

struct sProp {
	uint32_t ID;
	std::string name;
	pTypes type;
	void* pData;
};

class ppBase {
public:
    ppBase();

    unsigned int getID();
	std::vector<sProp>& getProperties();
	void addProperty(std::string name, pTypes type, void* pdata);

private:
	std::vector<sProp> vProps;
	static unsigned int uiID_Counter;
	unsigned int uiID;
};
