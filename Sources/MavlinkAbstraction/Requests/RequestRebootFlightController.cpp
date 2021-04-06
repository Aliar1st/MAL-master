#include "RequestRebootFlightController.h"

RequestRebootFlightController::RequestRebootFlightController(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseRebootFlightController* response)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger),
	  response(response)
{
}

RequestRebootFlightController::~RequestRebootFlightController()
{
}

void RequestRebootFlightController::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, 0);

	if (response)
	{
		response->success = success;
	}
}

const char* RequestRebootFlightController::GetName()
{
	return "RebootFlightController";
}

void RequestRebootFlightController::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	*commandID = MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN; // Command: Reboot or shutdown system components
	params[0] = 1;                                  // 0: Do nothing for autopilot, 1: Reboot autopilot, 2: Shutdown autopilot, 3: Reboot autopilot and keep it in the bootloader until upgraded.
	params[1] = 0;                                  // 0: Do nothing for onboard computer, 1: Reboot onboard computer, 2: Shutdown onboard computer, 3: Reboot onboard computer and keep it in the bootloader until upgraded.
	params[2] = 0;                                  // 0: Do nothing for camera, 1: Reboot onboard camera, 2: Shutdown onboard camera, 3: Reboot onboard camera and keep it in the bootloader until upgraded
	params[3] = 0;                                  // 0: Do nothing for mount (e.g. gimbal), 1: Reboot mount, 2: Shutdown mount, 3: Reboot mount and keep it in the bootloader until upgraded
	params[4] = 0;                                  // Reserved
	params[5] = 0;                                  // Reserved
	params[6] = -1;                                 // ID (e.g. camera ID -1 for all IDs)
}