#include "TestVehicleCommandTakeOff.h"

#include <iostream>
#include "Utilities/InputUtil.h"

const char* TestVehicleCommandTakeOff::GetName()
{
	return "takeOff";
}

const char* TestVehicleCommandTakeOff::GetDescription()
{
	return "Performs a take-off";
}

Vehicle_Command_Execution_Result TestVehicleCommandTakeOff::execute(Vehicle* vehicle)
{
	// Get the target altitude
	std::cout << "Altitude to take off to [m]: ";

	float altitude = InputUtil::ReadFloatFromConsole();

	ResponseTakeOff response;

	if (vehicle->TakeOff(&response, altitude) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}