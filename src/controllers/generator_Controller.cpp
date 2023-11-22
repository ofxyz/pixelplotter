#include "generator_Controller.h"
#include "generator_Plaids.h"

GeneratorController::GeneratorController()
	: Controller()
{
	// Manual Mapping ...

	mapObjectTypes["Plaids"] = createInstance<G_plaids>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("Generator");

}
