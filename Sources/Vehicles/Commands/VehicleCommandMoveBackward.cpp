#include "VehicleCommandMoveBackward.h"

VehicleCommandMoveBackward::VehicleCommandMoveBackward(float movementBackward)
	: VehicleCommandMoveBy(-movementBackward, 0.0f, 0.0f, 0.0f)
{
}

const char* VehicleCommandMoveBackward::GetName()
{
	return "MoveBackward";
}