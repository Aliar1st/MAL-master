#include "TestVehicleCommandStabilize.h"

const char* TestVehicleCommandStabilize::GetName()
{
	return "stabilize";
}

const std::vector<const char*> TestVehicleCommandStabilize::GetAlternativeNames()
{
	return {"break"};
}

const char* TestVehicleCommandStabilize::GetDescription()
{
	return "Changes the mode to STABILIZE";
}

Vehicle_Command_Execution_Result TestVehicleCommandStabilize::execute(Vehicle* vehicle)
{
	ResponseStabilize response;

	if (vehicle->Stabilize(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}