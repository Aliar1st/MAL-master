#include "RequestChangeMode.h"

RequestChangeMode::RequestChangeMode(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseChangeMode* response)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger),
	  response(response)
{
}

RequestChangeMode::~RequestChangeMode()
{
}

void RequestChangeMode::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, 0);

	if (response)
	{
		response->success = success;
	}
}

void RequestChangeMode::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	float mode = getMode();

	*commandID = MAV_CMD_DO_SET_MODE;           // Command: Change mode
	params[0] = 1;                              // Mode
	params[1] = mode;                           // Custom mode (defined by Ardupilot)
	params[2] = 0;                              // Custom submodule
	params[3] = 0;                              // Empty
	params[4] = 0;                              // Empty
	params[5] = 0;                              // Empty
	params[6] = 0;                              // Empty
}