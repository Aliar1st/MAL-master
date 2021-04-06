#include "VehicleCommandArm.h"

const char* VehicleCommandArm::GetName()
{
	return "Arm";
}

Vehicle_Command_Execution_Result VehicleCommandArm::execute(Vehicle* vehicle)
{
	// Arm
	ResponseArm response;

	if (vehicle->Arm(&response) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
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
		return responseGetDeviceDataHeartbeat.deviceData.IsArmed();
	});
}