#include "RequestTakeOff.h"

RequestTakeOff::RequestTakeOff(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseTakeOff* response, float altitude)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger),
	  response(response),
	  altitude(altitude)
{
}

RequestTakeOff::~RequestTakeOff()
{
}

void RequestTakeOff::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, 0);

	if (response)
	{
		response->success = success;
	}
}

const char* RequestTakeOff::GetName()
{
	return "TakeOff";
}

void RequestTakeOff::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	*commandID = MAV_CMD_NAV_TAKEOFF;           // Command: Take off
	params[0] = 0;                              // Minimum pitch (if airspeed sensor present), desired pitch without sensor
	params[1] = 0;                              // Empty
	params[2] = 0;                              // Empty
	params[3] = 0;                              // Yaw angle (if magnetometer present), ignored without magnetometer. NaN to use the current system yaw heading mode (e.g. yaw towards next waypoint, yaw to home, etc.).
	params[4] = 0;                              // Latitude
	params[5] = 0;                              // Longitude
	params[6] = altitude;                       // Altitude
}