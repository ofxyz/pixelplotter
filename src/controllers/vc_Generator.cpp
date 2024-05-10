#include "vc_Generator.h"
#include "generator_Stripes.h"
#include "generator_Gradient.h"
#include "generator_Dots.h"

GeneratorController::GeneratorController()
	: ppVectorController()
{
	// Manual Mapping ...

	mapObjectTypes["Stripes"]  = createInstance<G_stripes>;
	mapObjectTypes["Gradient"] = createInstance<G_gradient>;
	mapObjectTypes["Dots"] = createInstance<G_dots>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
		add(p.first);
	}

	generateMenuNames("Generators");

}

void GeneratorController::draw(int width, int height) {
	for (const auto & g : v_Objects) {
		g->draw();
	}
	setFresh(false);
}
