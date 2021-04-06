#include "TestVehicleCommandPosHold.h"

const char* TestVehicleCommandPosHold::GetName()
{
	return "posHold";
}

const char* TestVehicleCommandPosHold::GetDescription()
{
	return "Changes the mode to POSHOLD";
}

Vehicle_Command_Execution_Result TestVehicleCommandPosHold::execute(Vehicle* vehicle)
{
	ResponsePosHold response;

	if (vehicle->PosHold(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}