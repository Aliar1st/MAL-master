#pragma once

#include <climits>
#include <mutex>
#include <vector>
#include "Arguments/MavlinkAbstractionArguments.h"
#include "MAVLink.h"
#include "Responses/ResponseAltHold.h"
#include "Responses/ResponseArm.h"
#include "Responses/ResponseDisarm.h"
#include "Responses/ResponseGetDeviceDataAttitude.h"
#include "Responses/ResponseGetDeviceDataBase.h"
#include "Responses/ResponseGetDeviceDataExtendedSystemState.h"
#include "Responses/ResponseGetDeviceDataGPSGlobalOrigin.h"
#include "Responses/ResponseGetDeviceDataGlobalPosition.h"
#include "Responses/ResponseGetDeviceDataHeartbeat.h"
#include "Responses/ResponseGetDeviceDataLocalPositionNED.h"
#include "Responses/ResponseGetTimeOfBoot.h"
#include "Responses/ResponseGuided.h"
#include "Responses/ResponseLand.h"
#include "Responses/ResponseMoveBy.h"
#include "Responses/ResponseMoveTo.h"
#include "Responses/ResponsePosHold.h"
#include "Responses/ResponseRTL.h"
#include "Responses/ResponseRebootFlightController.h"
#include "Responses/ResponseStabilize.h"
#include "Responses/ResponseTakeOff.h"

/**
 * @brief	Enumerates all possible return values of the functions of MavlinkAbstraction.
 */
enum MavlinkAbstraction_Return
{
	MAVLINK_ABSTRACTION_RETURN_SUCCESS,
	MAVLINK_ABSTRACTION_RETURN_ERROR_ALREADY_STARTED,
	MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED,
	MAVLINK_ABSTRACTION_RETURN_ERROR_CONNECTION_TIMEOUT,
	MAVLINK_ABSTRACTION_RETURN_ERROR_ON_STARTED
};

class C2Handler;
class Logger;
class Request;
class MAVLinkConnection;

/**
 * @brief	The main Mavlink Abstraction Layer class
 *			It abstracts the MAVLink connection for easy use by the user.
 *			Furthermore, the MAVLink connection can either be established via a Serial Port, a UDP socket or a TCP socket.
 */
class MavlinkAbstraction
{
	friend class MAVLinkConnection;
	friend class C2Handler;

private:

	bool isStarted;
	std::recursive_mutex threadSafetyMutex;
	std::mutex requestHandlingMutex;

	Logger* logger;

	// Sub systems
	MAVLinkConnection* mavlinkConnection = nullptr;
	C2Handler* c2Handler = nullptr;

	/**
	 * @brief	Function to call whenever a MAVLink message is received.
	 *			This function is thread-safe.
	 *
	 * @param[in]	message		The received message.
	 */
	void onMAVLinkMessageReceived(mavlink_message_t* message);

	/**
	 * @brief	Sends a MAVLink message to the MAVLink connection
	 *			This function is thread-safe.
	 *
	 * @param[in]	message		The message to send. This object must be allocated dynamically using new.
	 */
	void sendMAVLinkMessage(mavlink_message_t* message);

	/**
	 * @brief	Handles a request to this Mavlink Abstraction. Blocks until the corresponding response was received (if there is any)
	 *			This function is partially thread-safe meaning that a request may give up the thread-safety to receive MAVLink messages
	 *			However, this function is completely thread-safe given only the public functions.
	 *
	 * @param[in]	request		Request to handle. This variable must be allocated dynamically using new as it is being deleted within this function
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return handleRequest(Request* request);

	/**
	 * @brief	Deletes the sub systems if they exist
	 *			If they existed before, all threads they contain will be joined
	 */
	void deleteSubSystems();

	/**
	 * @brief	Logs the arguments
	 *
	 * @param[in]	arguments	Arguments of the Mavlink Abstraction
	 */
	void logArguments(MavlinkAbstractionArguments& arguments);

	/**
	 * @brief	Resets the Mavlink Abstraction to its original state
	 */
	void reset();

protected:

	/**
	 * @brief	Called whenever this Mavlink Abstraction was started
	 *
	 * @return	True if no error occurred; False otherwise.
	 *			If false, stops the Mavlink Abstraction.
	 */
	virtual bool onStarted();

	/**
	 * @brief	Called whenever this Mavlink Abstraction was stopped
	 */
	virtual void onStopped();

public:

	constexpr static const unsigned int NO_TIMEOUT = std::numeric_limits<unsigned int>::max();

	/**
	 * @brief	The constructor of MavlinkAbstraction
	 */
	MavlinkAbstraction();

	/**
	 * @brief	The destructor of MavlinkAbstraction.
	 */
	~MavlinkAbstraction();

	/**
	 * @brief	Starts the Mavlink Abstraction by connecting to a MAVLink device
	 *			This function is thread-safe.
	 *
	 * @param[in]	arguments	Arguments of the Mavlink Abstraction
	 * @param		timeout		Timeout until the connection must be opened and until a MAVlink message has been received in milliseconds.
	 *							If MavlinkAbstraction::NO_TIMEOUT, no timeout occurs.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS						The Mavlink Abstraction was started successfully
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_ALREADY_STARTED		The Mavlink Abstraction is started already
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_CONNECTION_TIMEOUT		A timeout occurred before a connection could have been established
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_ON_STARTED				The onStarted() function failed and returned false
	 */
	MavlinkAbstraction_Return Start(MavlinkAbstractionArguments& arguments, unsigned int timeout);

	/**
	 * @brief	Stops the Mavlink Abstraction by disconnecting from the MAVLink device
	 *			This function is thread-safe.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS						The Mavlink Abstraction was stopped successfully
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED			The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Stop();

	/**
	 * @brief	Changes the mode to ALTHOLD. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return AltHold(ResponseAltHold* response);

	/**
	 * @brief	Arms the MAVLink device. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Arm(ResponseArm* response);

	/**
	 * @brief	Emits a sound
	 *			This function is thread-safe.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Beep();

	/**
	 * @brief	Disarms the MAVLink device. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 * @param		force		True to disarm by force; False otherwise
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Disarm(ResponseDisarm* response, bool force = false);

	/**
	 * @brief	Gets data of the MAVLink device. Blocks until the response(s) was/were received.
	 *			The given response objects determine which data is requested.
	 *			This function is thread-safe.
	 *
	 * @param[out]	responses	Vector containing all responses. The responses specifiy which data is to be retrieved.
	 *							May be empty. May contain NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return GetDeviceData(const std::vector<ResponseGetDeviceDataBase*>& responses);

	/**
	 * @brief	Gets the boot time of the MAVLink device. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return GetTimeOfBoot(ResponseGetTimeOfBoot* response);

	/**
	 * @brief	Changes the mode to GUIDED. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Guided(ResponseGuided* response);

	/**
	 * @brief	Changes the mode to LAND. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Land(ResponseLand* response);

	/**
	 * @brief	Moves the MAVLink device by an offset. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response			Response; May be NULL.
	 * @param		movementForward		Movement in heading direction [m]
	 * @param		movementRightward	Movement rightwards [m]
	 * @param		movementUpward		Movement upwards [m]
	 * @param		rotation			Heading rotation offset [deg] (clockwise)
	 */
	MavlinkAbstraction_Return MoveBy(ResponseMoveBy* response, float movementForward, float movementRightward, float movementUpward, float rotation);

	/**
	 * @brief	Moves the MAVLink device to an absolute position.
	 *			This function is thread-safe.
	 *
	 * @param[out]	response			Response; May be NULL.
	 * @param		targetLatitude		Target latitude [degE7]
	 * @param		targetLongitude		Target longitude [degE7]
	 * @param		targetAltitude		Relative altitude to home position [m]
	 * @param		targetHeading		Target heading [deg]
	 */
	MavlinkAbstraction_Return MoveTo(ResponseMoveTo* response, int32_t targetLatitude, int32_t targetLongitude, float targetAltitude, float targetHeading);

	/**
	 * @brief	Changes the mode to POSHOLD. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return PosHold(ResponsePosHold* response);

	/**
	 * @brief	Reboots the flight controller. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return RebootFlightController(ResponseRebootFlightController* response);

	/**
	 * @brief	Changes the mode to RTL. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return RTL(ResponseRTL* response);

	/**
	 * @brief	Changes the mode to STABILIZE. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return Stabilize(ResponseStabilize* response);

	/**
	 * @brief	Perform a take-off. Blocks until the response was received
	 *			This function is thread-safe.
	 *
	 * @param[out]	response	Response; May be NULL.
	 * @param		altitude	Altitude to take off to [m]
	 *
	 * @return	MAVLINK_ABSTRACTION_RETURN_SUCCESS				The request was successfully handled
	 *			MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED	The Mavlink Abstraction is not started
	 */
	MavlinkAbstraction_Return TakeOff(ResponseTakeOff* response, float altitude);

	/**
	 * @brief	Returns whether a Mavlink Abstaction is started.
	 *			Note that it will stop itself if an error occurs
	 *
	 * @return	True if the Mavlink Abstraction is started; False otherwise
	 */
	bool IsStarted();
};