#include "VehicleCommandDisarm.h"

const char* VehicleCommandDisarm::GetName()
{
	return "Disarm";
}

Vehicle_Command_Execution_Result VehicleCommandDisarm::execute(Vehicle* vehicle)
{
	// Disarm
	ResponseDisarm response;

	if (vehicle->Disarm(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	ResponseGetDeviceDataHeartbeat responseGetDeviceDataHeartbeat;

	// Wait until MAVLink device is armed
	return waitForDeviceDataCondition(
		vehicle,
		{&responseGetDeviceDataHeartbeat},
		[&]()
	{
		return responseGetDeviceDataHeartbeat.deviceData.IsDisarmed();
	});
}