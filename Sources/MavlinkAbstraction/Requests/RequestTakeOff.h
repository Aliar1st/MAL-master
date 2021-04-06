#pragma once

#include "MavlinkAbstraction/Responses/ResponseTakeOff.h"
#include "RequestCommand.h"

/**
 * @brief	Performs a take-off
 */
class RequestTakeOff : public RequestCommand
{
private:

	ResponseTakeOff* response;

	float altitude;

protected:

	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) override;

public:

	/**
	 * @brief	The constructor of RequestTakeOff
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 * @param		altitude			Altitude to take off to
	 */
	RequestTakeOff(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseTakeOff* response, float altitude);

	/**
	 * @brief	The destructor of RequestTakeOff.
	 */
	virtual ~RequestTakeOff();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual const char* GetName() override;
};