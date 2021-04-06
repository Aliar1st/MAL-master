#pragma once

#include "MavlinkAbstraction/Responses/ResponseGetDeviceDataLocalPositionNED.h"
#include "RequestGetDeviceDataBase.h"

/**
 * @brief	Class to handle the response of the "GetDeviceData" request which retrieves device data about the local position in the NED frame
 */
class RequestGetDeviceDataLocalPositionNED : public RequestGetDeviceDataBase
{
private:

	ResponseGetDeviceDataLocalPositionNED* response;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceDataLocalPositionNED.
	 *
	 * @param[in]	logger		Logger
	 * @param[in]	response	Response to write the data into
	 */
	RequestGetDeviceDataLocalPositionNED(Logger* logger, ResponseGetDeviceDataLocalPositionNED* response);

	/**
	 * @brief	The destructor of RequestGetDeviceDataLocalPositionNED.
	 */
	virtual ~RequestGetDeviceDataLocalPositionNED();

	virtual uint16_t GetRequestedMessageID() override;

	virtual void SetResponseSuccess(bool success) override;

	virtual void OnResponseReceived(mavlink_message_t* message) override;
};