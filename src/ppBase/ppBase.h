#pragma once

#include <string>
#include <vector>

typedef enum propTypes {
	EPT_BOOL = 1,
	EPT_INT,
	EPT_UINT,
	EPT_FLOAT,
	EPT_DOUBLE,
	EPT_STRING,
	EPT_IMVEC4,
	EPT_COUNT
} propTypes;

struct sProp {
	uint32_t id;
	std::string name;
	propTypes type;
	void* data;
};

class ppBase {
public:
    ppBase();

    unsigned int getID();

	std::vector<sProp>& getProperties();
	void addProperty(std::string name, propTypes type, void* data);

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool _isFresh = true;
	std::vector<sProp> vProps;
	static unsigned int uiID_Counter;
	unsigned int uiID;
};
