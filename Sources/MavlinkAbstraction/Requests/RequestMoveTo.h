#pragma once

#include "MavlinkAbstraction/Responses/ResponseMoveTo.h"
#include "RequestCommand.h"

/**
 * @brief	Moves to an absolute position
 */
class RequestMoveTo : public RequestCommand
{
private:

	constexpr static const unsigned int TIME_REQUESTED_TIMEOUT = 250;

	ResponseMoveTo* response;

	int32_t targetLatitude;
	int32_t targetLongitude;
	float targetAltitude;
	float targetHeading;

	uint32_t timeSinceBoot;
	bool timeSinceBootReceived;
	std::condition_variable_any timeSinceBootReceivedCondition;

protected:

	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) override;

public:

	/**
	 * @brief	The constructor of RequestMoveTo
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 * @param		targetLatitude		Target latitude [degE7]
	 * @param		targetLongitude		Target longitude [degE7]
	 * @param		targetAltitude		Relative altitude to home position [m]
	 * @param		targetHeading		Target heading [deg]
	 */
	RequestMoveTo(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseMoveTo* response, int32_t targetLatitude, int32_t targetLongitude, float targetAltitude, float targetHeading);

	/**
	 * @brief	The destructor of RequestMoveTo.
	 */
	virtual ~RequestMoveTo();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual void OnMAVLinkMessageReceived(mavlink_message_t* message) override;

	virtual const char* GetName() override;
};