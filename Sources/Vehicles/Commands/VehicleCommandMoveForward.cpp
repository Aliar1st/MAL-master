#include "VehicleCommandMoveForward.h"

VehicleCommandMoveForward::VehicleCommandMoveForward(float movementForward)
	: VehicleCommandMoveBy(movementForward, 0.0f, 0.0f, 0.0f)
{
}

const char* VehicleCommandMoveForward::GetName()
{
	return "MoveForward";
}