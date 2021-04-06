#include "RequestArm.h"

RequestArm::RequestArm(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseArm* response)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger),
	  response(response)
{
}

RequestArm::~RequestArm()
{
}

void RequestArm::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, 0);

	if (response)
	{
		response->success = success;
	}
}

const char* RequestArm::GetName()
{
	return "Arm";
}

void RequestArm::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	*commandID = MAV_CMD_COMPONENT_ARM_DISARM;  // Command: Arm/Disarm
	params[0] = 1;                              // Arm
	params[1] = 0;                              // Don't force
	params[2] = 0;                              // Empty
	params[3] = 0;                              // Empty
	params[4] = 0;                              // Empty
	params[5] = 0;                              // Empty
	params[6] = 0;                              // Empty
}