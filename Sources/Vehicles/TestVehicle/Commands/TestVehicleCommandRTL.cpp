#include "TestVehicleCommandRTL.h"

const char* TestVehicleCommandRTL::GetName()
{
	return "rtl";
}

const char* TestVehicleCommandRTL::GetDescription()
{
	return "Changes the mode to RTL";
}

Vehicle_Command_Execution_Result TestVehicleCommandRTL::execute(Vehicle* vehicle)
{
	ResponseRTL response;

	if (vehicle->RTL(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}
