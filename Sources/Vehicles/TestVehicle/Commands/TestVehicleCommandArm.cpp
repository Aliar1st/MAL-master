#include "TestVehicleCommandArm.h"

const char* TestVehicleCommandArm::GetName()
{
	return "arm";
}

const char* TestVehicleCommandArm::GetDescription()
{
	return "Arms the MAVLink device";
}

Vehicle_Command_Execution_Result TestVehicleCommandArm::execute(Vehicle* vehicle)
{
	ResponseArm response;

	if (vehicle->Arm(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}