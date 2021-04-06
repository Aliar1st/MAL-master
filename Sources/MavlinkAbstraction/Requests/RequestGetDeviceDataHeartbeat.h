#pragma once

#include "MavlinkAbstraction/Responses/ResponseGetDeviceDataHeartbeat.h"
#include "RequestGetDeviceDataBase.h"

/**
 * @brief	Class to handle the response of the "GetDeviceData" request which retrieves device data about the heartbeat
 */
class RequestGetDeviceDataHeartbeat : public RequestGetDeviceDataBase
{
private:

	ResponseGetDeviceDataHeartbeat* response;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceDataHeartbeat.
	 *
	 * @param[in]	logger		Logger
	 * @param[in]	response	Response to write the data into
	 */
	RequestGetDeviceDataHeartbeat(Logger* logger, ResponseGetDeviceDataHeartbeat* response);

	/**
	 * @brief	The destructor of RequestGetDeviceDataHeartbeat.
	 */
	virtual ~RequestGetDeviceDataHeartbeat();

	virtual uint16_t GetRequestedMessageID() override;

	virtual void SetResponseSuccess(bool success) override;

	virtual void OnResponseReceived(mavlink_message_t* message) override;
};