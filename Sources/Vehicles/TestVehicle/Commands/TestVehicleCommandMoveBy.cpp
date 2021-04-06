#include "TestVehicleCommandMoveBy.h"

#include <iostream>
#include "Utilities/InputUtil.h"

const char* TestVehicleCommandMoveBy::GetName()
{
	return "moveBy";
}

const char* TestVehicleCommandMoveBy::GetDescription()
{
	return "Moves by an offset";
}

Vehicle_Command_Execution_Result TestVehicleCommandMoveBy::execute(Vehicle* vehicle)
{
	// Forward movement
	std::cout << "Forward movement [m]: ";

	float movementForward = InputUtil::ReadFloatFromConsole();

	// Rightward movement
	std::cout << "Rightward movement[m]: ";

	float movementRightward = InputUtil::ReadFloatFromConsole();

	// Upward movement
	std::cout << "Upward movement [m]: ";

	float movementUpward = InputUtil::ReadFloatFromConsole();

	// Heading
	std::cout << "Heading to turn by [deg] (clockwise): ";

	float rotation = InputUtil::ReadFloatFromConsole();

	ResponseMoveBy response;

	if (vehicle->MoveBy(&response, movementForward, movementRightward, movementUpward, rotation) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	return Vehicle_Command_Execution_Result::SUCCESS;
}
