#pragma once

#include <condition_variable>
#include "MavlinkAbstraction/Responses/ResponseGetTimeOfBoot.h"
#include "Request.h"

/**
 * @brief	Gets the time of boot of the MAVLink device
 *			This request uses the TIMESTAMP message. A few assumptions are made:
 *				- The timestamp provided by the MAVLink device is in nanoseconds
 *				- The timestamp provided by the MAVLink device is the time since the boot
 */
class RequestGetTimeOfBoot : public Request
{
private:

	constexpr static const unsigned int TIMESYNC_RESPONSE_TIMEOUT = 250;

	ResponseGetTimeOfBoot* response;

	bool timesyncResponseReceived;
	std::condition_variable_any timesyncResponseReceivedCondition;

	/**
	 * @brief	Returns the current timestamp in microseconds since the UNIX epoch time
	 *
	 * @return	Current timestamp
	 */
	uint64_t getCurrentTimestamp();

public:

	/**
	 * @brief	The constructor of RequestGetTimeOfBoot
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestGetTimeOfBoot(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseGetTimeOfBoot* response);

	/**
	 * @brief	The destructor of RequestGetTimeOfBoot.
	 */
	virtual ~RequestGetTimeOfBoot();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual void OnMAVLinkMessageReceived(mavlink_message_t* message) override;

	virtual const char* GetName() override;
};