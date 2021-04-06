#include "TestVehicleCommandKill.h"

const char* TestVehicleCommandKill::GetName()
{
	return "kill";
}

const std::vector<const char*> TestVehicleCommandKill::GetAlternativeNames()
{
	return {"k"};
}

const char* TestVehicleCommandKill::GetDescription()
{
	return "Disarms the MAVLink device by force";
}

Vehicle_Command_Execution_Result TestVehicleCommandKill::execute(Vehicle* vehicle)
{
	ResponseDisarm response;

	if (vehicle->Disarm(&response, true) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}