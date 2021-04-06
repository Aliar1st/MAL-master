#include "TestVehicleCommandLand.h"

const char* TestVehicleCommandLand::GetName()
{
	return "land";
}

const char* TestVehicleCommandLand::GetDescription()
{
	return "Changes the mode to LAND";
}

Vehicle_Command_Execution_Result TestVehicleCommandLand::execute(Vehicle* vehicle)
{
	ResponseLand response;

	if (vehicle->Land(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}