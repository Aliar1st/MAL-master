#include "TestVehicleCommandMission6.h"

#include "Vehicles/Commands/VehicleCommandArm.h"
#include "Vehicles/Commands/VehicleCommandGuided.h"
#include "Vehicles/Commands/VehicleCommandLand.h"
#include "Vehicles/Commands/VehicleCommandMoveLeftward.h"
#include "Vehicles/Commands/VehicleCommandMoveRightward.h"
#include "Vehicles/Commands/VehicleCommandMoveUpward.h"
#include "Vehicles/Commands/VehicleCommandTakeOff.h"

const char* TestVehicleCommandMission6::GetName()
{
	return "mission6";
}

const char* TestVehicleCommandMission6::GetDescription()
{
	return "Takes off, moves, ascends, moves, lands";
}

const std::vector<VehicleCommand*> TestVehicleCommandMission6::getCommands(Vehicle* vehicle)
{
	const float multiplier = (vehicle->IsOutdoor() ? 10.0f : 1.0f);

	const float takeOffAltitude = TAKE_OFF_ALTITUDE * multiplier;
	const float rightLeftMovement = RIGHT_LEFT_MOVEMENT * multiplier;
	const float upDownMovement = UP_DOWN_MOVEMENT * multiplier;

	return std::vector<VehicleCommand*>
		   {
			   new VehicleCommandGuided(),
			   new VehicleCommandArm(),
			   new VehicleCommandTakeOff(takeOffAltitude),
			   new VehicleCommandMoveRightward(rightLeftMovement),
			   new VehicleCommandMoveLeftward(2 * rightLeftMovement),
			   new VehicleCommandMoveRightward(rightLeftMovement),
			   new VehicleCommandMoveUpward(upDownMovement),
			   new VehicleCommandMoveRightward(rightLeftMovement),
			   new VehicleCommandMoveLeftward(2 * rightLeftMovement),
			   new VehicleCommandMoveRightward(rightLeftMovement),
			   new VehicleCommandLand()
		   };
}