#include "VehicleCommandMoveLeftward.h"

VehicleCommandMoveLeftward::VehicleCommandMoveLeftward(float movementLeftward)
	: VehicleCommandMoveBy(0.0f, -movementLeftward, 0.0f, 0.0f)
{
}

const char* VehicleCommandMoveLeftward::GetName()
{
	return "MoveLeftward";
}