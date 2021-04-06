#include "VehicleCommandLand.h"

const char* VehicleCommandLand::GetName()
{
	return "Land";
}

Vehicle_Command_Execution_Result VehicleCommandLand::execute(Vehicle* vehicle)
{
	// Change mode to LAND
	ResponseLand response;

	if (vehicle->Land(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	ResponseGetDeviceDataExtendedSystemState responseGetDeviceDataExtendedSystemState;

	// Wait until landed
	return waitForDeviceDataCondition(
		vehicle,
		{&responseGetDeviceDataExtendedSystemState},
		[&]()
	{
		return responseGetDeviceDataExtendedSystemState.deviceData.IsLandedStateOnGround();
	});
}