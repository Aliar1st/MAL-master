#include "TestVehicleCommandRebootFlightController.h"

const char* TestVehicleCommandRebootFlightController::GetName()
{
	return "rebootFlightController";
}

const std::vector<const char*> TestVehicleCommandRebootFlightController::GetAlternativeNames()
{
	return {"reboot"};
}

const char* TestVehicleCommandRebootFlightController::GetDescription()
{
	return "Reboots the flight controller";
}

Vehicle_Command_Execution_Result TestVehicleCommandRebootFlightController::execute(Vehicle* vehicle)
{
	ResponseRebootFlightController response;

	if (vehicle->RebootFlightController(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}