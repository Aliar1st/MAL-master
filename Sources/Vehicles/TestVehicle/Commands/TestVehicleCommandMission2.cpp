#include "TestVehicleCommandMission2.h"

#include "Vehicles/Commands/VehicleCommandArm.h"
#include "Vehicles/Commands/VehicleCommandGuided.h"
#include "Vehicles/Commands/VehicleCommandLand.h"
#include "Vehicles/Commands/VehicleCommandMoveDownward.h"
#include "Vehicles/Commands/VehicleCommandMoveUpward.h"
#include "Vehicles/Commands/VehicleCommandTakeOff.h"

const char* TestVehicleCommandMission2::GetName()
{
	return "mission2";
}

const char* TestVehicleCommandMission2::GetDescription()
{
	return "Takes off, descend, ascend, lands";
}

const std::vector<VehicleCommand*> TestVehicleCommandMission2::getCommands(Vehicle* vehicle)
{
	const float multiplier = (vehicle->IsOutdoor() ? 10.0f : 1.0f);

	const float takeOffAltitude = TAKE_OFF_ALTITUDE * multiplier;
	const float upDownMovement = UP_DOWN_MOVEMENT * multiplier;

	return std::vector<VehicleCommand*>
		   {
			   new VehicleCommandGuided(),
			   new VehicleCommandArm(),
			   new VehicleCommandTakeOff(takeOffAltitude),
			   new VehicleCommandMoveDownward(upDownMovement),
			   new VehicleCommandMoveUpward(upDownMovement),
			   new VehicleCommandLand()
		   };
}