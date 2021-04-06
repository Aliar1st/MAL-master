#include "TestVehicleCommandGuided.h"

const char* TestVehicleCommandGuided::GetName()
{
	return "guided";
}

const char* TestVehicleCommandGuided::GetDescription()
{
	return "Changes the mode to GUIDED";
}

Vehicle_Command_Execution_Result TestVehicleCommandGuided::execute(Vehicle* vehicle)
{
	ResponseGuided response;

	if (vehicle->Guided(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}