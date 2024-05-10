#pragma once
#include "ofJson.h"
#include "ofx2d.h"
#include <imgui.h>
#include <imgui_internal.h>

/*
	The Controller class is a managing factory of class t
	class t will contain the following functions:
	- void loadSettings(ofJson s)
	- ofJson getSettings()
	- void update()   // Update will be called when:
	- bool isFresh()  // Check if there are changes for update() to be called
	- bool isAlive()  // If isAlive returns false object t will be deleted from object vector 
*/

class ofApp;

template<class t>
class ppVectorController {
public:
	ofApp* pixelplotter{ nullptr };
	ppVectorController();

	std::vector<std::string> v_objectNames;
	std::vector<std::shared_ptr<t>> v_Objects;
	std::vector<std::string> v_menuValues;
	
	typedef std::map<std::string, std::shared_ptr<t>(*)()> map_type;
	map_type mapObjectTypes;

	template<typename o> static std::shared_ptr<t> createInstance() { return std::make_shared<o>(); };

	void update();
	void renderImGuiSettings();
	void loadSettings(ofJson& settings);
	ofJson getSettings();

	void add(std::string name, ofJson settings = {});
	void add(ofJson settings); // TODO refactor. ofJson is also a string :(
	void addRandom();
	void reorder();
	void duplicate();
	void clean();
	void clear();

	bool isFresh();
	void setFresh(bool fresh);

	void generateMenuNames(std::string objectName);

private:
	bool _bFresh;
	bool _bClean;
	bool _bReorder;
	bool _bDuplicate;

	int _currAddIndex;
	std::string _objectName;
	std::string _address;
};

template<class t>
ppVectorController<t>::ppVectorController()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	_bFresh = false;
	_bClean = false;
	_bReorder = false;
	_currAddIndex = 0;
	_objectName = "Undefined";
	_address = std::to_string((unsigned long long)(void**)this);
}

template<class t>
void ppVectorController<t>::add(std::string name, ofJson settings /*= {}*/)
{
	if (mapObjectTypes.count(name) > 0) {
		v_Objects.push_back(mapObjectTypes[name]());
		if (!settings.empty()) {
			v_Objects[v_Objects.size() - 1]->loadSettings(settings);
		}
		setFresh(true);
	}
	else {
		ofLog(OF_LOG_WARNING) << "Controller: Could not find name " << name << " in mapObjectTypes";
	}
}

template<class t>
void ppVectorController<t>::add(ofJson settings)
{
	try {
		add(settings.value("name", "not_found"), settings);
		setFresh(true);
	}
	catch (...) {
		ofLog(OF_LOG_ERROR) << "Failed to add Object with name " << settings.value("name", "not_found");
		return;
	}
}

template<class t>
void ppVectorController<t>::addRandom()
{
	add(v_objectNames[ofRandom(1, v_objectNames.size())]);
}

template<class t>
ofJson ppVectorController<t>::getSettings()
{
	ofJson settings;
	for (int i = 0; i < v_Objects.size(); i++) {
		settings[_objectName.c_str()].push_back(v_Objects[i]->getSettings());
	}
	return settings;
}

template<class t>
void ppVectorController<t>::loadSettings(ofJson& settings)
{
	try {
		ofJson objectSettings = settings.value(_objectName.c_str(), ofJson::array());
		if (!objectSettings.empty()) {
			for (auto & objectSetting : objectSettings) {
				add(objectSetting);
			}
		}
	}
	catch (...) {
		ofLog(OF_LOG_WARNING) << "Could not load presets for Object Name: " << _objectName.c_str() << std::endl;
	}
}

template<class t>
void ppVectorController<t>::update()
{
	if (_bClean) {
		clean();
	}
	if (_bReorder) {
		reorder();
	}
	if (_bDuplicate) {
		duplicate();
	}

	for (const auto& o : v_Objects) {
		if (o->isFresh()) {
			o->setFresh(false);
			setFresh(true);
		}
	}
}

template<class t>
void ppVectorController<t>::renderImGuiSettings()
{
	ImGui::PushID(_address.c_str());

	std::string sHeaderName = _objectName + " (" + ofToString(v_Objects.size()) + ")";
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);

	if (ImGui::CollapsingHeader(sHeaderName.c_str()))
	{
		ImGui::Indent();
		if (ofxImGui::VectorCombo("##AddSelector", &_currAddIndex, v_menuValues))
		{
			add(v_menuValues[_currAddIndex]);
			_currAddIndex = 0;
		}
		ImGui::Unindent();

		_bClean     = false;
		_bReorder   = false;
		_bDuplicate = false;

		// Order Top Down to Reflect Drawing order
		for (int i = v_Objects.size() - 1; i >= 0; i--) {
			if (v_Objects[i]->isAlive()) {
				ImGui::Indent();
				ImGui::PushID(i);

				v_Objects[i]->renderImGuiSettings();
				// We will need to refactor this at some point
				// collapsing headers are not the best for drag and drop
				// And we should be more free with ->renderImGuiSettings()?
	
				// BeginDragDropSource() allows dragging an item
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					// Set payload to contain the index of the object being dragged
					int index = i; // Using i directly since it's already in reverse order
					ImGui::SetDragDropPayload("OBJECT_INDEX", &index, sizeof(int));
					ImGui::Text("Drag to reorder");
					ImGui::EndDragDropSource();
				}

				// BeginDragDropTarget() allows dropping onto this item
				if (ImGui::BeginDragDropTarget()) {
					// Accept payload of type "OBJECT_INDEX" and reorder objects accordingly
					if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("OBJECT_INDEX")) {
						IM_ASSERT(payload->DataSize == sizeof(int));
						int payloadIndex = *(const int *)payload->Data;
						int currentPos = i;

						if (payloadIndex != currentPos) {
							ofx2d::move(v_Objects, payloadIndex, currentPos);
							setFresh(true);
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::PopID();
				ImGui::Unindent();
			} else {
				_bClean = true;
			}
			if (v_Objects[i]->duplicate) {
				_bDuplicate = true;
			}
		}


	}
	ImGui::PopID();
}

// TODO: This will need to change after implementing ImGui drag drop
template<class t>
void ppVectorController<t>::reorder()
{
	return;
	for (int i = 0; i < v_Objects.size(); i++) {
		if (v_Objects[i]->moveUp) {
			v_Objects[i]->moveUp = false;
			setFresh(true);
			if (i < v_Objects.size() - 1) {
				ofx2d::move(v_Objects, i, i + 1);
			}
		}
		else if (v_Objects[i]->moveDown) {
			v_Objects[i]->moveDown = false;
			setFresh(true);
			if (i > 0) {
				ofx2d::move(v_Objects, i, i - 1);
			}
		}
	}
};

template <class t>
void ppVectorController<t>::duplicate()
{
	for (int i = 0; i < v_Objects.size(); i++) {
		if (v_Objects[i]->duplicate) {
			v_Objects[i]->duplicate = false;
			add(v_Objects[i]->getSettings());
			setFresh(true);
		}
	}
}

template<class t>
void ppVectorController<t>::clean()
{
	for (int i = 0; i < v_Objects.size(); i++) {
		if (!v_Objects[i]->isAlive()) {
			v_Objects[i] = nullptr;
			setFresh(true);
		}
	}
	v_Objects.erase(std::remove(v_Objects.begin(), v_Objects.end(), nullptr), v_Objects.end());
};

template<class t>
void ppVectorController<t>::clear()
{
	v_Objects.clear();
	setFresh(true);
};

template<class t>
bool ppVectorController<t>::isFresh()
{
	return _bFresh;
};

template<class t>
void ppVectorController<t>::setFresh(bool fresh)
{
	_bFresh = fresh;
};

template<class t>
void ppVectorController<t>::generateMenuNames(std::string objectName)
{
	v_menuValues.clear();
	std::string menuValueInit = "Add " + objectName + " ...";
	v_menuValues.push_back(menuValueInit);
	for (std::string v : v_objectNames) {
		v_menuValues.push_back(v);
	}
	_objectName = objectName;
};
 