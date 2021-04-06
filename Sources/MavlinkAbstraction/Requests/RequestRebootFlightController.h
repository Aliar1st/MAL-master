#pragma once

#include "MavlinkAbstraction/Responses/ResponseRebootFlightController.h"
#include "RequestCommand.h"

/**
 * @brief	Reboots the flight controller
 */
class RequestRebootFlightController : public RequestCommand
{
private:

	ResponseRebootFlightController* response;

protected:

	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) override;

public:

	/**
	 * @brief	The constructor of RequestRebootFlightController
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestRebootFlightController(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseRebootFlightController* response);

	/**
	 * @brief	The destructor of RequestRebootFlightController.
	 */
	virtual ~RequestRebootFlightController();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual const char* GetName() override;
};