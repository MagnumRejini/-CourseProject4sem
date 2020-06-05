#pragma once

#include "engine.h"

typedef PSI float;

class Turbo : public Engine
{
private:
	PSI air_pressure;
	float ar;
public:
	Turbo(0) : air_pressure(0), ar(0), Engine(0) {}
	
};
