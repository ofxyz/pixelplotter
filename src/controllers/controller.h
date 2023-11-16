#pragma once

class ofApp;

template<class t>
class Controller {
public:
	ofApp* pixelplotter{ nullptr };
	Controller();

	std::vector<std::string> v_objectNames;
	std::vector<std::shared_ptr<t>> v_Objects;
	std::vector<std::string> v_menuValues;
	
	void move(std::vector<t>& v, size_t oldIndex, size_t newIndex);
	
	void renderImGuiSettings();

	void reorder();
	void clean();
	void clear();

	bool isFresh();
	void setFresh(bool fresh);

	std::string _objectsTypeName = "-";
	std::string _menuValueInit = "Add " + _objectsTypeName + " ...";

private:
	bool _bFresh   = false;
	bool _bClean   = false;
	bool _bReorder = false;

	int _currAddIndex = 0;

	void generateMenuNames();
};

template<class t>
void Controller<t>::renderImGuiSettings()
{
	ImGui::PushID(to_string(&this));

	std::string sHeaderName = _objectsTypeName + " (" + ofToString(v_Objects.size()) + ")";
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
			ImGui::PushID(i);
			if (v_Objects[i]->active) {
				ImGui::Indent();
				v_Objects[i]->renderImGuiSettings();
				ImGui::Unindent();
			}
			else {
				_bClean = true;
			}
			if (v_Objects[i]->moveUp || v_Objects[i]->moveDown) {
				_bReorder = true;
			}
			ImGui::PopID();
		}

		// ImGui::PopStyleColor(10);

		ImGui::Indent();
		if (ofxImGui::VectorCombo("##AddSelector", &_currAddIndex, v_MenuValues))
		{
			add(v_menuValues[_currAddIndex]);
			_currAddIndex = 0;
		}
		ImGui::Unindent();

	}
	ImGui::PopID();
}

template<class t>
Controller<t>::Controller()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	_menuValueInit = "-";
};

template<class t>
void Controller<t>::move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
	if (oldIndex > newIndex)
		std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
	else
		std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
};

// TODO: This will need to change after implementing ImGui drag drop
template<class t>
void Controller<t>::reorder()
{
	for (int i = 0; i < v_Objects.size(); i++) {
		if (v_Objects[i]->moveUp) {
			v_Objects[i]->moveUp = false;
			setFresh(true);
			if (i > 0) {
				move(v_Objects, i, i - 1);
			}
		}
		else if (v_Objects[i]->moveDown) {
			v_Objects[i]->moveDown = false;
			setFresh(true);
			if (i < v_Objects.size() - 1) {
				move(v_Objects, i, i + 1);
			}
		}
	}
};

template<class t>
void Controller<t>::clean()
{
	for (int i = 0; i < v_Objects.size(); i++) {
		// TODO: isActive()
		if (!v_Objects[i]->active) {
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
void Controller<t>::generateMenuNames()
{
	v_menuValues.clear();
	v_menuValues.push_back(menuValueInit);
	for (std::string v : v_objectNames) {
		v_menuValues.push_back(v);
	}
};
