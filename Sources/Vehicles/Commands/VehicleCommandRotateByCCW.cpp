#include "VehicleCommandRotateByCCW.h"

VehicleCommandRotateByCCW::VehicleCommandRotateByCCW(float rotation)
	: VehicleCommandMoveBy(0.0f, 0.0f, 0.0f, -rotation)
{
}

const char* VehicleCommandRotateByCCW::GetName()
{
	return "RotateByCCW";
}