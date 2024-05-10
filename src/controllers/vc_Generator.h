#pragma once
#include "ppVectorController.h"
#include "generator.h"

class GeneratorController : public ppVectorController<Generator>
{
	public:
		GeneratorController();
		void draw(int width, int height);
};
