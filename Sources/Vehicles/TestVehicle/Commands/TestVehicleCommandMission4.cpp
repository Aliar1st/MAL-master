#include "TestVehicleCommandMission4.h"

#include "Vehicles/Commands/VehicleCommandArm.h"
#include "Vehicles/Commands/VehicleCommandGuided.h"
#include "Vehicles/Commands/VehicleCommandLand.h"
#include "Vehicles/Commands/VehicleCommandMoveUpward.h"
#include "Vehicles/Commands/VehicleCommandRotateByCCW.h"
#include "Vehicles/Commands/VehicleCommandRotateByCW.h"
#include "Vehicles/Commands/VehicleCommandTakeOff.h"

const char* TestVehicleCommandMission4::GetName()
{
	return "mission4";
}

const char* TestVehicleCommandMission4::GetDescription()
{
	return "Takes off, moves, lands";
}

const std::vector<VehicleCommand*> TestVehicleCommandMission4::getCommands(Vehicle* vehicle)
{
	const float multiplier = (vehicle->IsOutdoor() ? 10.0f : 1.0f);

	const float takeOffAltitude = TAKE_OFF_ALTITUDE * multiplier;
	const float upDownMovement = UP_DOWN_MOVEMENT * multiplier;
	const float frontBackMovement = FRONT_BACK_MOVEMENT * multiplier;

	return std::vector<VehicleCommand*>
		   {
			   new VehicleCommandGuided(),
			   new VehicleCommandArm(),
			   new VehicleCommandTakeOff(takeOffAltitude),
			   new VehicleCommandMoveBy(frontBackMovement, 0.0f, upDownMovement, 0.0f),
			   new VehicleCommandLand()
		   };
}