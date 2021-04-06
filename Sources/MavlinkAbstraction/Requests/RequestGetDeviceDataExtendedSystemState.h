#pragma once

#include "MavlinkAbstraction/Responses/ResponseGetDeviceDataExtendedSystemState.h"
#include "RequestGetDeviceDataBase.h"

/**
 * @brief	Class to handle the response of the "GetDeviceData" request which retrieves device data about the extended system state
 */
class RequestGetDeviceDataExtendedSystemState : public RequestGetDeviceDataBase
{
private:

	ResponseGetDeviceDataExtendedSystemState* response;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceDataExtendedSystemState.
	 *
	 * @param[in]	logger		Logger
	 * @param[in]	response	Response to write the data into
	 */
	RequestGetDeviceDataExtendedSystemState(Logger* logger, ResponseGetDeviceDataExtendedSystemState* response);

	/**
	 * @brief	The destructor of RequestGetDeviceDataExtendedSystemState.
	 */
	virtual ~RequestGetDeviceDataExtendedSystemState();

	virtual uint16_t GetRequestedMessageID() override;

	virtual void SetResponseSuccess(bool success) override;

	virtual void OnResponseReceived(mavlink_message_t* message) override;
};