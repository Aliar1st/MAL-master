#include "VehicleCommandMoveDownward.h"

VehicleCommandMoveDownward::VehicleCommandMoveDownward(float movementDownward)
	: VehicleCommandMoveBy(0.0f, 0.0f, -movementDownward, 0.0f)
{
}

const char* VehicleCommandMoveDownward::GetName()
{
	return "MoveDownward";
}