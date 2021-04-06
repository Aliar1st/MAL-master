#include "TestVehicleCommandMoveTo.h"

#include <iostream>
#include "Utilities/InputUtil.h"

const char* TestVehicleCommandMoveTo::GetName()
{
	return "moveTo";
}

const char* TestVehicleCommandMoveTo::GetDescription()
{
	return "Moves to an absolute position";
}

Vehicle_Command_Execution_Result TestVehicleCommandMoveTo::execute(Vehicle* vehicle)
{
	// Latitude
	std::cout << "Latitude to move to [degE7]: ";

	int32_t targetLatitude = InputUtil::ReadIntFromConsole();

	// Longitude
	std::cout << "Longitude to move to [degE7]: ";

	int32_t targetLongitude = InputUtil::ReadIntFromConsole();

	// Longitude
	std::cout << "Altitude to ascend/descend to [m]: ";

	float targetAltitude = InputUtil::ReadFloatFromConsole();

	// Heading
	std::cout << "Heading to turn to [deg]: ";

	float targetHeading = InputUtil::ReadFloatFromConsole();

	ResponseMoveTo response;

	if (vehicle->MoveTo(&response, targetLatitude, targetLongitude, targetAltitude, targetHeading) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}
