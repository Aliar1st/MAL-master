#include "VehicleCommandRotateByCW.h"

VehicleCommandRotateByCW::VehicleCommandRotateByCW(float rotation)
	: VehicleCommandMoveBy(0.0f, 0.0f, 0.0f, rotation)
{
}

const char* VehicleCommandRotateByCW::GetName()
{
	return "RotateByCW";
}