#pragma once

#include "Request.h"

/**
 * @brief	Emits a sound
 */
class RequestBeep : public Request
{
public:

	/**
	 * @brief	The constructor of RequestBeep
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 */
	RequestBeep(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger);

	/**
	 * @brief	The destructor of RequestBeep.
	 */
	virtual ~RequestBeep();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual const char* GetName() override;
};