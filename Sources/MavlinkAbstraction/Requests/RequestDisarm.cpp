#include "RequestDisarm.h"

RequestDisarm::RequestDisarm(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseDisarm* response, bool force)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger),
	  response(response),
	  force(force)
{
}

RequestDisarm::~RequestDisarm()
{
}

void RequestDisarm::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, 0);

	if (response)
	{
		response->success = success;
	}
}

const char* RequestDisarm::GetName()
{
	return "Disarm";
}

void RequestDisarm::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	float forceFlag = (force ? 21196.0f : 0.0f);

	*commandID = MAV_CMD_COMPONENT_ARM_DISARM;  // Command: Arm/Disarm
	params[0] = 0;                              // Disarm
	params[1] = forceFlag;                      // Force flag
	params[2] = 0;                              // Empty
	params[3] = 0;                              // Empty
	params[4] = 0;                              // Empty
	params[5] = 0;                              // Empty
	params[6] = 0;                              // Empty
}