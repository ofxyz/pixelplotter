#pragma once
#include "ofJson.h"
#include "ofx2d.h"

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
class Controller {
public:
	ofApp* pixelplotter{ nullptr };
	Controller();

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
	void add(ofJson settings);
	void addRandom();
	void reorder();
	void clean();
	void clear();

	bool isFresh();
	void setFresh(bool fresh);

	void generateMenuNames(std::string objectName);

private:
	bool _bFresh;
	bool _bClean;
	bool _bReorder;
	int _currAddIndex;
	std::string _objectName;
	std::string _address;
};

template<class t>
Controller<t>::Controller()
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
void Controller<t>::add(std::string name, ofJson settings /*= {}*/)
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
void Controller<t>::add(ofJson settings)
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
void Controller<t>::addRandom()
{
	add(v_objectNames[ofRandom(1, v_objectNames.size())]);
}

template<class t>
ofJson Controller<t>::getSettings()
{
	ofJson settings;
	for (int i = 0; i < v_Objects.size(); i++) {
		settings[_objectName.c_str()].push_back(v_Objects[i]->getSettings());
	}
	return settings;
}

template<class t>
void Controller<t>::loadSettings(ofJson& settings)
{
	ofJson objectSettings = settings.value(_objectName.c_str(), ofJson::array());
	if (!objectSettings.empty()) {
		for (auto& objectSetting : objectSettings) {
			add(objectSetting);
		}
	}
}

template<class t>
void Controller<t>::update()
{
	if (_bClean) {
		clean();
	}
	if (_bReorder) {
		reorder();
	}

	for (const auto& o : v_Objects) {
		if (o->isFresh()) {
			o->setFresh(false);
			setFresh(true);
		}
	}
}

template<class t>
void Controller<t>::renderImGuiSettings()
{
	ImGui::PushID(_address.c_str());

	std::string sHeaderName = _objectName + " (" + ofToString(v_Objects.size()) + ")";
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(sHeaderName.c_str()))
	{
		/* We need to make this a bit more controllable using settings ...
		ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0, 0, 0.4));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(0, 0, 0.7));

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0, 0.7));

		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0, 0, 0.4));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0, 0, 0.5));

		ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4)ImColor::HSV(0, 0, 0.8));
		*/
		_bClean   = false;
		_bReorder = false;

		for (int i = 0; i < v_Objects.size(); i++) {
			if (v_Objects[i]->isAlive()) {
				ImGui::Indent();
				ImGui::PushID(i);
				v_Objects[i]->renderImGuiSettings();
				ImGui::PopID();
				ImGui::Unindent();
			}
			else {
				_bClean = true;
			}
			if (v_Objects[i]->moveUp || v_Objects[i]->moveDown) {
				_bReorder = true;
			}
		}

		// ImGui::PopStyleColor(10);

		ImGui::Indent();
		if (ofxImGui::VectorCombo("##AddSelector", &_currAddIndex, v_menuValues))
		{
			add(v_menuValues[_currAddIndex]);
			_currAddIndex = 0;
		}
		ImGui::Unindent();

	}
	ImGui::PopID();
}

// TODO: This will need to change after implementing ImGui drag drop
template<class t>
void Controller<t>::reorder()
{
	for (int i = 0; i < v_Objects.size(); i++) {
		if (v_Objects[i]->moveUp) {
			v_Objects[i]->moveUp = false;
			setFresh(true);
			if (i > 0) {
				ofx2d::move(v_Objects, i, i - 1);
			}
		}
		else if (v_Objects[i]->moveDown) {
			v_Objects[i]->moveDown = false;
			setFresh(true);
			if (i < v_Objects.size() - 1) {
				ofx2d::move(v_Objects, i, i + 1);
			}
		}
	}
};

template<class t>
void Controller<t>::clean()
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
void Controller<t>::clear()
{
	v_Objects.clear();
	setFresh(true);
};

template<class t>
bool Controller<t>::isFresh()
{
	return _bFresh;
};

template<class t>
void Controller<t>::setFresh(bool fresh)
{
	_bFresh = fresh;
};

template<class t>
void Controller<t>::generateMenuNames(std::string objectName)
{
	v_menuValues.clear();
	std::string menuValueInit = "Add " + objectName + " ...";
	v_menuValues.push_back(menuValueInit);
	for (std::string v : v_objectNames) {
		v_menuValues.push_back(v);
	}
	_objectName = objectName;
};
 