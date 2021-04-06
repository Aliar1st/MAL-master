#include "VehicleCommandTakeOff.h"

#include "Utilities/MathUtil.h"

VehicleCommandTakeOff::VehicleCommandTakeOff(float altitude)
	: altitude(altitude)
{
}

const char* VehicleCommandTakeOff::GetName()
{
	return "TakeOff";
}

Vehicle_Command_Execution_Result VehicleCommandTakeOff::execute(Vehicle* vehicle)
{
	// Perform a take-off
	ResponseTakeOff response;

	if (vehicle->TakeOff(&response, altitude) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	// Calculate target altitude
	const int32_t targetAltitude = static_cast<int32_t>(altitude * 1000.0f);

	ResponseGetDeviceDataGlobalPosition responseGetDeviceDataGlobalPosition;

	// Wait until altitude reached
	return waitForDeviceDataCondition(
		vehicle,
		{&responseGetDeviceDataGlobalPosition},
		[&]()
	{
		return MathUtil::DistanceBetweenAltitudes(responseGetDeviceDataGlobalPosition.deviceData.relativeAltitude, targetAltitude) <= ALTITUDE_THRESHOLD;
	});
}