#include "TestVehicleCommandMission3.h"

#include "Vehicles/Commands/VehicleCommandArm.h"
#include "Vehicles/Commands/VehicleCommandGuided.h"
#include "Vehicles/Commands/VehicleCommandLand.h"
#include "Vehicles/Commands/VehicleCommandMoveForward.h"
#include "Vehicles/Commands/VehicleCommandTakeOff.h"

const char* TestVehicleCommandMission3::GetName()
{
	return "mission3";
}

const char* TestVehicleCommandMission3::GetDescription()
{
	return "Takes off, moves forward, lands";
}

const std::vector<VehicleCommand*> TestVehicleCommandMission3::getCommands(Vehicle* vehicle)
{
	const float multiplier = (vehicle->IsOutdoor() ? 10.0f : 1.0f);

	const float takeOffAltitude = TAKE_OFF_ALTITUDE * multiplier;
	const float frontBackMovement = FRONT_BACK_MOVEMENT * multiplier;

	return std::vector<VehicleCommand*>
		   {
			   new VehicleCommandGuided(),
			   new VehicleCommandArm(),
			   new VehicleCommandTakeOff(takeOffAltitude),
			   new VehicleCommandMoveForward(frontBackMovement),
			   new VehicleCommandLand()
		   };
}