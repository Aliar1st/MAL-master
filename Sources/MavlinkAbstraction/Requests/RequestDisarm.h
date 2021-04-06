#pragma once

#include "MavlinkAbstraction/Responses/ResponseDisarm.h"
#include "RequestCommand.h"

/**
 * @brief	Disarms the MAVLink device
 */
class RequestDisarm : public RequestCommand
{
private:

	ResponseDisarm* response;

	bool force;

protected:

	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) override;

public:

	/**
	 * @brief	The constructor of RequestDisarm
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 * @param		force				True to disarm by force; False otherwise
	 */
	RequestDisarm(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseDisarm* response, bool force);

	/**
	 * @brief	The destructor of RequestDisarm.
	 */
	virtual ~RequestDisarm();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual const char* GetName() override;
};