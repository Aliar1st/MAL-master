#pragma once

#include "MavlinkAbstraction/Responses/ResponseGetDeviceDataAttitude.h"
#include "RequestGetDeviceDataBase.h"

/**
 * @brief	Class to handle the response of the "GetDeviceData" request which retrieves device data about the attitude
 */
class RequestGetDeviceDataAttitude : public RequestGetDeviceDataBase
{
private:

	ResponseGetDeviceDataAttitude* response;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceDataAttitude.
	 *
	 * @param[in]	logger		Logger
	 * @param[in]	response	Response to write the data into
	 */
	RequestGetDeviceDataAttitude(Logger* logger, ResponseGetDeviceDataAttitude* response);

	/**
	 * @brief	The destructor of RequestGetDeviceDataAttitude.
	 */
	virtual ~RequestGetDeviceDataAttitude();

	virtual uint16_t GetRequestedMessageID() override;

	virtual void SetResponseSuccess(bool success) override;

	virtual void OnResponseReceived(mavlink_message_t* message) override;
};