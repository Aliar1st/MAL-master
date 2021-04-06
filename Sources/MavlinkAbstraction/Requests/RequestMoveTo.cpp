#include "RequestMoveTo.h"

#include "MavlinkAbstraction/C2Handler/C2Handler.h"
#include "Utilities/MathUtil.h"

RequestMoveTo::RequestMoveTo(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseMoveTo* response, int32_t targetLatitude, int32_t targetLongitude, float targetAltitude, float targetHeading)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger),
	  response(response),
	  targetLatitude(targetLatitude),
	  targetLongitude(targetLongitude),
	  targetAltitude(targetAltitude),
	  targetHeading(targetHeading)
{
}

RequestMoveTo::~RequestMoveTo()
{
}

void RequestMoveTo::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	// Request time
	timeSinceBootReceived = false;

	// Send the request command until the time was received
	while (!timeSinceBootReceived && mavlinkAbstraction->IsStarted())
	{
		// Request time
		bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, 0);

		if (!success)
		{
			if (response)
			{
				response->success = false;
			}

			return;
		}

		// Wait until the time is received or a timeout occurred
		std::cv_status timeWaitResult = std::cv_status::no_timeout;

		while (!timeSinceBootReceived && timeWaitResult == std::cv_status::no_timeout)
		{
			timeWaitResult = timeSinceBootReceivedCondition.wait_for(uniqueLock, std::chrono::milliseconds(TIME_REQUESTED_TIMEOUT));
		}
	}

	const uint8_t coordinateFrame = MAV_FRAME_GLOBAL_RELATIVE_ALT_INT;

	// Ignore velocities, accelerations and heading turn speed
	const uint16_t typeMask = POSITION_TARGET_TYPEMASK_VX_IGNORE | POSITION_TARGET_TYPEMASK_VY_IGNORE | POSITION_TARGET_TYPEMASK_VZ_IGNORE | POSITION_TARGET_TYPEMASK_AX_IGNORE | POSITION_TARGET_TYPEMASK_AY_IGNORE | POSITION_TARGET_TYPEMASK_AZ_IGNORE | POSITION_TARGET_TYPEMASK_YAW_RATE_IGNORE;

	// Convert to radians
	const float targetHeadingRad = MathUtil::ToRadians(targetHeading);

	// Define the message
	mavlink_message_t* message = new mavlink_message_t;

	mavlink_msg_set_position_target_global_int_pack(
		ownSystemID,        // Own system ID
		ownComponentID,     // Own component ID
		message,            // MAVLink message to pack the data into
		timeSinceBoot,      // Time since boot [ms]
		targetSystemID,     // Target's system ID
		0,                  // Target's component ID (broadcast)
		coordinateFrame,    // Desired coordinate frame to work with
		typeMask,           // Type mask indicating which fields should be ignored
		targetLatitude,     // The target latitude [degE7]
		targetLongitude,    // The target longitude [degE7]
		targetAltitude,     // The target altitude [m]
		0,                  // X velocity (ignored)
		0,                  // Y velocity (ignored)
		0,                  // Z velocity (ignored)
		0,                  // X acceleration (ignored)
		0,                  // Y acceleration (ignored)
		0,                  // Z acceleration (ignored)
		targetHeadingRad,   // Target heading direction [rad]
		0                   // Heading rate (ignored)
	);

	// Send the message
	c2Handler->SendMAVLinkMessage(message);

	if (response)
	{
		response->success = true;
	}
}

void RequestMoveTo::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Handle acknowledgements
	RequestCommand::OnMAVLinkMessageReceived(message);

	switch (message->msgid)
	{
		case MAVLINK_MSG_ID_SYSTEM_TIME:
		{
			// Decode message
			mavlink_system_time_t decodedMessage;

			mavlink_msg_system_time_decode(message, &decodedMessage);

			// Set time
			timeSinceBoot = decodedMessage.time_boot_ms;

			// Time was received
			timeSinceBootReceived = true;
			timeSinceBootReceivedCondition.notify_one();

			break;
		}
	}
}

const char* RequestMoveTo::GetName()
{
	return "MoveTo";
}

void RequestMoveTo::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	*commandID = MAV_CMD_REQUEST_MESSAGE;               // Command: Request message
	params[0] = MAVLINK_MSG_ID_SYSTEM_TIME;             // MAVLink message to request
	params[1] = 0;                                      // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[2] = 0;                                      // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[3] = 0;                                      // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[4] = 0;                                      // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[5] = 0;                                      // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[6] = 0;                                      // Target address of requested messages (0: Flight-stack, 1: Address of requestor, 2: broadcast)
}