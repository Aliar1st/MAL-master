#include "VehicleCommandMoveUpward.h"

VehicleCommandMoveUpward::VehicleCommandMoveUpward(float movementUpward)
	: VehicleCommandMoveBy(0.0f, 0.0f, movementUpward, 0.0f)
{
}

const char* VehicleCommandMoveUpward::GetName()
{
	return "MoveUpward";
}