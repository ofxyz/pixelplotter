#include "generator_Controller.h"
#include "generator.h"
#include "generator_Plaids.h"
#include "ofx2d.h"

GeneratorController::GeneratorController()
{
	pixelplotter = (ofApp*)ofGetAppPtr();

	v_ObjectNames = {
	"Plaids"
	};

	generateMenuNames();

}

template <typename t> void GeneratorController::move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
	if (oldIndex > newIndex)
		std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
	else
		std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

void GeneratorController::generateMenuNames()
{
	v_MenuValues.clear();
	v_MenuValues.push_back("Add Generator ...");
	for (std::string s : v_ObjectNames) {
		v_MenuValues.push_back(s);
	}
}

void GeneratorController::reorder()
{
	for (int i = 0; i < v_Generators.size(); i++) {
		if (v_Generators[i]->moveUp) {
			v_Generators[i]->moveUp = false;
			v_Generators[i]->setFresh(true);
			if (i > 0) {
				move(v_Generators, i, i - 1);
			}
		}
		else if (v_Generators[i]->moveDown) {
			v_Generators[i]->moveDown = false;
			v_Generators[i]->setFresh(true);
			if (i < v_Generators.size() - 1) {
				move(v_Generators, i, i + 1);
			}
		}
	}
}

void GeneratorController::addGenerator(ofJson& filterSettings)
{
	string filterName = filterSettings.value("name", "not_found");
	if (filterName == "Plaids") {
		v_Generators.push_back(new G_plaids(pixelplotter));
		v_Generators[v_Generators.size() - 1]->loadSettings(filterSettings);
	}
}

void GeneratorController::addGenerator(string name)
{
	if (name == "Plaids") {
		v_Generators.push_back(new G_plaids(pixelplotter));
	}
}

void GeneratorController::addGenerator(int index)
{
	if (v_ObjectNames[index] == "Plaids") {
		v_Generators.push_back(new G_plaids(pixelplotter));
	}
}

void GeneratorController::addGenerator(Generator* generator)
{
	v_Generators.push_back(generator);
}

void GeneratorController::addRandomGenerator()
{
	addGenerator(ofRandom(1, v_ObjectNames.size() - 1));
}

void GeneratorController::cleanFilters()
{
	for (int i = 0; i < v_Generators.size(); i++) {
		if (!v_Generators[i]->active) {
			delete v_Generators[i];
			v_Generators[i] = nullptr;
		}
	}
	v_Generators.erase(std::remove(v_Generators.begin(), v_Generators.end(), nullptr), v_Generators.end());
}

void GeneratorController::clearFilters()
{
	for (int i = 0; i < v_Generators.size(); i++) {
		delete v_Generators[i];
		v_Generators[i] = nullptr;
	}
	v_Generators.clear();
}
