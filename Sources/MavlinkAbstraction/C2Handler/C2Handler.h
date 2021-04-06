#pragma once

#include <condition_variable>
#include <stdint.h>
#include "MavlinkAbstraction/MavlinkAbstraction.h"

/**
 * @brief	A class that evaluates read MAVLink messages and requests to the Mavlink Abstraction by the user
 */
class C2Handler
{
private:

	constexpr static const uint8_t COMPONENT_ID = 1;

	bool isStarted;
	uint8_t ownSystemID;
	uint8_t targetSystemID;

	MavlinkAbstraction* mavlinkAbstraction;

	Logger* logger;

	// Current request
	Request* request;

	/**
	 * @brief	Resets the C2 Handler to its original state
	 */
	void reset();

public:

	/**
	 * @brief	The constructor of C2Handler
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this C2Handler belongs to
	 * @param[in]	logger				Logger
	 */
	C2Handler(MavlinkAbstraction* mavlinkAbstraction, Logger* logger);

	/**
	 * @brief	The destructor of C2Handler.
	 */
	~C2Handler();

	/**
	 * @brief	Starts the C2 Handler
	 *
	 * @param[in]	arguments	Arguments of the Mavlink Abstraction
	 */
	void Start(MavlinkAbstractionArguments& arguments);

	/**
	 * @brief	Disconnects from the MAVLink device
	 */
	void Stop();

	/**
	 * @brief	Handles a request
	 *
	 * @param[in]	request		Request to handle.
	 * @param[in]	uniqueLock	Unique lock used by the Mavlink Abstraction. Unlocking this allows other threads to use the Mavlink Abstraction and is required for Request::OnMAVLinkMessageReceived to be called
	 *
	 * @return	Response of the request. NULL if the request has no response
	 */
	void HandleRequest(Request* request, std::unique_lock<std::recursive_mutex>& uniqueLock);

	/**
	 * @brief	Sends a MAVLink message to the MAVLink connection
	 *
	 * @param[in]	message		The message to send. This object must be allocated dynamically using new.
	 */
	void SendMAVLinkMessage(mavlink_message_t* message);

	/**
	 * @brief	Function to call whenever a MAVLink message is received.
	 *
	 * @param[in]	message		The received message.
	 */
	void OnMAVLinkMessageReceived(mavlink_message_t* message);
};