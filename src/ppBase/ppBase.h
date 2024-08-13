#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ofJson.h"

class ppBaseManager;

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

    unsigned int getId();

	std::vector<sProp>& getProperties();
	void addProperty(std::string name, propTypes type, void* data);

	bool isFresh();
	void setFresh(bool fresh);

	bool isAlive();
	void setAlive(bool alive);

	std::shared_ptr<ppBaseManager> GetEntityManager() const { return m_pBaseManager; }
	void SetEntityManager(std::shared_ptr<ppBaseManager> sp) { m_pBaseManager = sp; }

private:
	bool m_isFresh = true;
	bool m_isAlive = true;
	std::vector<sProp> m_vProps;
	static unsigned int m_uiIdCounter;
	unsigned int m_uiId;

	std::shared_ptr<ppBaseManager> m_pBaseManager;
};
