#include "VehicleCommandMoveRightward.h"

VehicleCommandMoveRightward::VehicleCommandMoveRightward(float movementRightward)
	: VehicleCommandMoveBy(0.0f, movementRightward, 0.0f, 0.0f)
{
}

const char* VehicleCommandMoveRightward::GetName()
{
	return "MoveRightward";
}