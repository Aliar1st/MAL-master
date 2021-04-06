#pragma once

#include "MavlinkAbstraction/Responses/ResponseGetDeviceDataGPSGlobalOrigin.h"
#include "RequestGetDeviceDataBase.h"

/**
 * @brief	Class to handle the response of the "GetDeviceData" request which retrieves device data about the GPS global origin
 *
 *			TODO: Replace the internal logic to handle MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN once the ArduPilot firmware correctly uses the GPS origin as the origin of the Local NED Frame instead of the home position (when https://github.com/ArduPilot/ardupilot/pull/15169 is merged into ArduCopter)
 */
class RequestGetDeviceDataGPSGlobalOrigin : public RequestGetDeviceDataBase
{
private:

	ResponseGetDeviceDataGPSGlobalOrigin* response;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceDataGPSGlobalOrigin.
	 *
	 * @param[in]	logger		Logger
	 * @param[in]	response	Response to write the data into
	 */
	RequestGetDeviceDataGPSGlobalOrigin(Logger* logger, ResponseGetDeviceDataGPSGlobalOrigin* response);

	/**
	 * @brief	The destructor of RequestGetDeviceDataGPSGlobalOrigin.
	 */
	virtual ~RequestGetDeviceDataGPSGlobalOrigin();

	virtual uint16_t GetRequestedMessageID() override;

	virtual void SetResponseSuccess(bool success) override;

	virtual void OnResponseReceived(mavlink_message_t* message) override;
};