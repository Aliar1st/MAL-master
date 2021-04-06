#include "VehicleCommandGuided.h"

const char* VehicleCommandGuided::GetName()
{
	return "Guided";
}

Vehicle_Command_Execution_Result VehicleCommandGuided::execute(Vehicle* vehicle)
{
	// Change mode to GUIDED
	ResponseGuided response;

	if (vehicle->Guided(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	ResponseGetDeviceDataHeartbeat responseGetDeviceDataHeartbeat;

	// Wait until the mode is changed
	return waitForDeviceDataCondition(
		vehicle,
		{&responseGetDeviceDataHeartbeat},
		[&]()
	{
		return responseGetDeviceDataHeartbeat.deviceData.IsModeGuided();
	});
}