#include "TestVehicleCommandDisarm.h"

const char* TestVehicleCommandDisarm::GetName()
{
	return "disarm";
}

const char* TestVehicleCommandDisarm::GetDescription()
{
	return "Disarms the MAVLink device";
}

Vehicle_Command_Execution_Result TestVehicleCommandDisarm::execute(Vehicle* vehicle)
{
	ResponseDisarm response;

	if (vehicle->Disarm(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}