#include "VehicleCommandMoveBy.h"

#include "Utilities/MathUtil.h"

VehicleCommandMoveBy::VehicleCommandMoveBy(float movementForward, float movementRightward, float movementUpward, float rotation)
	: movementForward(movementForward),
	  movementRightward(movementRightward),
	  movementUpward(movementUpward),
	  rotation(rotation)
{
}

const char* VehicleCommandMoveBy::GetName()
{
	return "MoveBy";
}

Vehicle_Command_Execution_Result VehicleCommandMoveBy::execute(Vehicle* vehicle)
{
	// Move
	ResponseMoveBy response;

	if (vehicle->MoveBy(&response, movementForward, movementRightward, movementUpward, rotation) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!response.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	// Get target position
	const int32_t targetLatitude = response.targetLatitude;
	const int32_t targetLongitude = response.targetLongitude;
	const int32_t targetAltitude = response.targetAltitude;
	const uint16_t targetHeading = response.targetHeading;

	ResponseGetDeviceDataGlobalPosition responseGetDeviceDataGlobalPosition;

	// Wait until the target position is reached
	return waitForDeviceDataCondition(
		vehicle,
		{&responseGetDeviceDataGlobalPosition},
		[&]()
	{
		bool horizontalDistanceSatisfied = MathUtil::DistanceBetweenGeographicPoints(responseGetDeviceDataGlobalPosition.deviceData.latitude, responseGetDeviceDataGlobalPosition.deviceData.longitude, targetLatitude, targetLongitude) <= HORIZONTAL_MOVEMENT_THRESHOLD;
		bool verticalDistanceSatisfied = MathUtil::DistanceBetweenAltitudes(responseGetDeviceDataGlobalPosition.deviceData.relativeAltitude, targetAltitude) <= VERTICAL_MOVEMENT_THRESHOLD;
		bool rotationDistanceSatisfied = MathUtil::DistanceBetweenAngles(responseGetDeviceDataGlobalPosition.deviceData.heading, targetHeading) <= ROTATION_THRESHOLD;

		return horizontalDistanceSatisfied && verticalDistanceSatisfied && rotationDistanceSatisfied;
	});
}