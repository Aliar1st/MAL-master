#include "TestVehicleCommandMission8.h"

#include "Vehicles/Commands/VehicleCommandArm.h"
#include "Vehicles/Commands/VehicleCommandGuided.h"
#include "Vehicles/Commands/VehicleCommandLand.h"
#include "Vehicles/Commands/VehicleCommandMoveBy.h"
#include "Vehicles/Commands/VehicleCommandTakeOff.h"

const char* TestVehicleCommandMission8::GetName()
{
	return "mission8";
}

const char* TestVehicleCommandMission8::GetDescription()
{
	return "Takes off, moves three times, lands";
}

const std::vector<VehicleCommand*> TestVehicleCommandMission8::getCommands(Vehicle* vehicle)
{
	const float multiplier = (vehicle->IsOutdoor() ? 10.0f : 1.0f);

	const float takeOffAltitude = TAKE_OFF_ALTITUDE * multiplier;
	const float rightLeftMovement = RIGHT_LEFT_MOVEMENT * multiplier;
	const float frontBackMovement = FRONT_BACK_MOVEMENT * multiplier;

	return std::vector<VehicleCommand*>
		   {
			   new VehicleCommandGuided(),
			   new VehicleCommandArm(),
			   new VehicleCommandTakeOff(takeOffAltitude),
			   new VehicleCommandMoveBy(-frontBackMovement, -rightLeftMovement, 0.0f, 0.0f),
			   new VehicleCommandMoveBy(2 * frontBackMovement, 0.0f, 0.0f, 0.0f),
			   new VehicleCommandMoveBy(-frontBackMovement, rightLeftMovement, 0.0f, 0.0f),
			   new VehicleCommandLand()
		   };
}