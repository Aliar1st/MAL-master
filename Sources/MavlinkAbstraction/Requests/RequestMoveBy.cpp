#include "RequestMoveBy.h"

#include "MavlinkAbstraction/C2Handler/C2Handler.h"
#include "MavlinkAbstraction/Requests/RequestMoveTo.h"
#include "RequestGetDeviceData.h"
#include "RequestGetDeviceDataGlobalPosition.h"
#include "Utilities/MathUtil.h"

RequestMoveBy::RequestMoveBy(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseMoveBy* response, float movementForward, float movementRightward, float movementUpward, float rotation)
	: Request(mavlinkAbstraction, c2Handler, logger),
	  response(response),
	  movementForward(movementForward),
	  movementRightward(movementRightward),
	  movementUpward(movementUpward),
	  rotation(rotation)
{
}

RequestMoveBy::~RequestMoveBy()
{
}

void RequestMoveBy::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	// Request position
	ResponseGetDeviceDataGlobalPosition responseGlobalPosition;

	executeSubRequest(new RequestGetDeviceData(mavlinkAbstraction, c2Handler, logger, {&responseGlobalPosition}), uniqueLock, ownSystemID, ownComponentID, targetSystemID);

	// Check if failed
	if (!responseGlobalPosition.success)
	{
		if (response)
		{
			response->success = false;
		}

		return;
	}

	// Calculate offsets
	double headingRadians = MathUtil::ToRadians(responseGlobalPosition.deviceData.heading / 100.0);

	double sinHeading = sin(headingRadians);
	double cosHeading = cos(headingRadians);

	double offsetNorth = static_cast<float>(cosHeading * movementForward - sinHeading * movementRightward);
	double offsetEast = static_cast<float>(sinHeading * movementForward + cosHeading * movementRightward);

	// Calculate target position
	int32_t targetLatitude = responseGlobalPosition.deviceData.latitude + static_cast<int32_t>(MathUtil::ToDegree(offsetNorth / MathUtil::EARTH_RADIUS) * 1.0e7);
	int32_t targetLongitude = responseGlobalPosition.deviceData.longitude + static_cast<int32_t>(MathUtil::ToDegree(offsetEast / MathUtil::EARTH_RADIUS) / cos(MathUtil::ToRadians(responseGlobalPosition.deviceData.latitude / 1.0e7)) * 1.0e7);
	float targetAltitude = static_cast<float>(responseGlobalPosition.deviceData.relativeAltitude) / 1000.0f + movementUpward;
	float targetHeading = responseGlobalPosition.deviceData.heading / 100.0f + rotation;

	// Move to the position
	ResponseMoveTo moveToResponse;

	executeSubRequest(new RequestMoveTo(mavlinkAbstraction, c2Handler, logger, &moveToResponse, targetLatitude, targetLongitude, targetAltitude, targetHeading), uniqueLock, ownSystemID, ownComponentID, targetSystemID);

	if (response)
	{
		// Bring target heaading into [0; 360) to allow conversion to uint16_t
		targetHeading = MathUtil::Modulo(targetHeading, 360.0f);

		response->success = moveToResponse.success;
		response->targetLatitude = targetLatitude;
		response->targetLongitude = targetLongitude;
		response->targetAltitude = static_cast<int32_t>(targetAltitude * 1000.0f);
		response->targetHeading = static_cast<uint16_t>(targetHeading * 100.0f);
	}
}

const char* RequestMoveBy::GetName()
{
	return "MoveBy";
}