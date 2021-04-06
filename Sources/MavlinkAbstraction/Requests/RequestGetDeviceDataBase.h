#pragma once

#include <stdint.h>
#include "MavlinkAbstraction/MAVLink.h"

class Logger;

/**
 * @brief	Base class to handle a single response of the "GetDeviceData" request
 *
 * @tparam	ResponseType	Type of the response
 */
class RequestGetDeviceDataBase
{
protected:

	Logger* logger;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceDataBase.
	 *
	 * @param[in]	logger		Logger
	 */
	RequestGetDeviceDataBase(Logger* logger);

	/**
	 * @brief	The destructor of RequestGetDeviceDataBase.
	 */
	virtual ~RequestGetDeviceDataBase();

	/**
	 * @brief	Returns the ID of the requested MAVLink message
	 *
	 * @return	MAVLink message ID
	 */
	virtual uint16_t GetRequestedMessageID() = 0;

	/**
	 * @brief	Sets whether the response was successful
	 *
	 * @param	success		True if the response was successful; False otherwise
	 */
	virtual void SetResponseSuccess(bool success) = 0;

	/**
	 * @brief	Function to call whenever the response corresponding to the request is received
	 *
	 * @param[in]	message		The received message.
	 */
	virtual void OnResponseReceived(mavlink_message_t* message) = 0;
};