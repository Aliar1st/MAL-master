#include "TestVehicleCommandAltHold.h"

const char* TestVehicleCommandAltHold::GetName()
{
	return "altHold";
}

const char* TestVehicleCommandAltHold::GetDescription()
{
	return "Changes the mode to ALTHOLD";
}

Vehicle_Command_Execution_Result TestVehicleCommandAltHold::execute(Vehicle* vehicle)
{
	ResponseAltHold response;

	if (vehicle->AltHold(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}