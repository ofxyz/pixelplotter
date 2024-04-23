#pragma once
#include "controller.h"
#include "generator.h"

class GeneratorController : public Controller<Generator>
{
	public:
		GeneratorController();
		void draw(int width, int height);
};
