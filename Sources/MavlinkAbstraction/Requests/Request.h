#pragma once

#include <mutex>
#include "MavlinkAbstraction/MAVLink.h"

class MavlinkAbstraction;
class C2Handler;
class Logger;

struct Response;

/**
 * @brief	A request to the Mavlink Abstraction
 */
class Request
{
private:

	Request* subRequest = nullptr;

protected:

	MavlinkAbstraction* mavlinkAbstraction;
	C2Handler* c2Handler;
	Logger* logger;

	/**
	 * @brief	Executes a given sub request
	 *
	 * @param[in]	subRequest		Sub request to execute. Must be allocated dynamically using new.
	 * @param[in]	uniqueLock		Unique lock used by the Mavlink Abstraction. Unlocking this allows other threads to use the Mavlink Abstraction and is required for OnMAVLinkMessageReceived to be called
	 * @param		ownSystemID		Own system ID
	 * @param		componentID		Own component ID
	 * @param		targetSystemID	Target's system ID
	 */
	void executeSubRequest(Request* subRequest, std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID);

public:

	/**
	 * @brief	The constructor of Request
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 */
	Request(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger);

	/**
	 * @brief	The destructor of Request.
	 */
	virtual ~Request();

	/**
	 * @brief	Executes the request
	 *
	 * @param[in]	uniqueLock		Unique lock used by the Mavlink Abstraction. Unlocking this allows other threads to use the Mavlink Abstraction and is required for OnMAVLinkMessageReceived to be called
	 * @param		ownSystemID		Own system ID
	 * @param		componentID		Own component ID
	 * @param		targetSystemID	Target's system ID
	 */
	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) = 0;

	/**
	 * @brief	Function to call whenever a MAVLink message is received while executing this request
	 *
	 * @param[in]	message		The received message.
	 */
	virtual void OnMAVLinkMessageReceived(mavlink_message_t* message);

	/**
	 * @brief	Returns the name of this request
	 *
	 * @return	Name
	 */
	virtual const char* GetName() = 0;
};