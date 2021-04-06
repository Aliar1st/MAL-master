#include "RequestGetTimeOfBoot.h"

#include <chrono>
#include "MavlinkAbstraction/C2Handler/C2Handler.h"
#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetTimeOfBoot::RequestGetTimeOfBoot(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseGetTimeOfBoot* response)
	: Request(mavlinkAbstraction, c2Handler, logger),
	  response(response)
{
}

RequestGetTimeOfBoot::~RequestGetTimeOfBoot()
{
}

void RequestGetTimeOfBoot::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	timesyncResponseReceived = false;

	bool timesyncSent = false;

	// Send the MAVLink command until the timesync response message has been received
	while (!timesyncResponseReceived && mavlinkAbstraction->IsStarted())
	{
		// Log
		if (timesyncSent)
		{
			*logger << logger->StartLogging(Logger_Mode::INFO) << "Resending timesync message" << logger->EndLogging();
		}

		// Define the message
		mavlink_message_t* message = new mavlink_message_t;

		mavlink_msg_timesync_pack(
			ownSystemID,                                // Own system ID
			ownComponentID,                             // Own component ID
			message,                                    // MAVLink message to pack the data into
			0,                                          // Time sync timestamp 1. Must be 0 because it is filled out by the MAVLink device
			static_cast<int64_t>(getCurrentTimestamp()) // Time sync timestamp 2. Current timestamp
		);

		// Send the message
		c2Handler->SendMAVLinkMessage(message);
		timesyncSent = true;

		// Wait until the response has been received or a timeout occurred
		std::cv_status timesyncResponseWaitResult = std::cv_status::no_timeout;

		while (!timesyncResponseReceived && timesyncResponseWaitResult == std::cv_status::no_timeout)
		{
			timesyncResponseWaitResult = timesyncResponseReceivedCondition.wait_for(uniqueLock, std::chrono::milliseconds(TIMESYNC_RESPONSE_TIMEOUT));
		}
	}
}

void RequestGetTimeOfBoot::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Handle sub requests
	Request::OnMAVLinkMessageReceived(message);

	// Check if timesync message
	if (message->msgid != MAVLINK_MSG_ID_TIMESYNC)
	{
		return;
	}

	// Take current timestamp
	uint64_t currentTimestamp = getCurrentTimestamp();

	// Decode the acknowledgement
	mavlink_timesync_t decodedMessage;

	mavlink_msg_timesync_decode(message, &decodedMessage);

	// Log
	logger->StartLogging(Logger_Mode::INFO);
	*logger << "Handling received MAVLink message of type: timesync" << Logger::Endl;
	*logger << Logger::Indent << "Timestamp 1: " << decodedMessage.tc1 << Logger::Endl;
	*logger << Logger::Indent << "Timestamp 2: " << decodedMessage.ts1 << Logger::Endl;
	logger->EndLogging();

	// Fill response object with data
	if (response)
	{
		// Calculate average timestamp
		uint64_t averageTimestamp = static_cast<uint64_t>(decodedMessage.ts1) + (currentTimestamp - static_cast<uint64_t>(decodedMessage.ts1)) / 2;

		// Calculate time of boot (time since boot sent by MAVLink device is in nanoseconds)
		response->timeOfBoot = averageTimestamp - (decodedMessage.tc1 / 1'000 + (decodedMessage.tc1 % 1'000 >= 500));
	}

	// Notify about the received timesync response
	timesyncResponseReceived = true;
	timesyncResponseReceivedCondition.notify_one();
}

const char* RequestGetTimeOfBoot::GetName()
{
	return "GetTimeOfBoot";
}

uint64_t RequestGetTimeOfBoot::getCurrentTimestamp()
{
	return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}