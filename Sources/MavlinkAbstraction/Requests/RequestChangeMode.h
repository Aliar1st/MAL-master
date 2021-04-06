#pragma once

#include "MavlinkAbstraction/Responses/ResponseChangeMode.h"
#include "RequestCommand.h"

/**
 * @brief	Changes mode
 */
class RequestChangeMode : public RequestCommand
{
private:

	ResponseChangeMode* response;

protected:

	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) override;

	/**
	 * @brief	Returns the mode to change to
	 *
	 * @return	Mode to change to
	 */
	virtual float getMode() = 0;

public:

	/**
	 * @brief	The constructor of RequestChangeMode
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestChangeMode(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseChangeMode* response);

	/**
	 * @brief	The destructor of RequestChangeMode.
	 */
	virtual ~RequestChangeMode();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;
};