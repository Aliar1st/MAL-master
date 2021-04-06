#include "C2Handler.h"

#include <cassert>
#include "MavlinkAbstraction/Logger/Logger.h"
#include "MavlinkAbstraction/Requests/Request.h"

C2Handler::C2Handler(MavlinkAbstraction* mavlinkAbstraction, Logger* logger)
	: mavlinkAbstraction(mavlinkAbstraction),
	  logger(logger)
{
	reset();
}

C2Handler::~C2Handler()
{
	Stop();
}

void C2Handler::Start(MavlinkAbstractionArguments& arguments)
{
	// Check if started already
	if (isStarted)
	{
		return;
	}

	isStarted = true;

	// Setup both system IDs
	ownSystemID = arguments.ownSystemID;
	targetSystemID = arguments.targetSystemID;
}

void C2Handler::Stop()
{
	// Check if started
	if (!isStarted)
	{
		return;
	}

	reset();
}

void C2Handler::HandleRequest(Request* request, std::unique_lock<std::recursive_mutex>& uniqueLock)
{
	this->request = request;

	// Execute the request
	request->Execute(uniqueLock, ownSystemID, COMPONENT_ID, targetSystemID);

	// Reset request
	this->request = nullptr;
}

void C2Handler::SendMAVLinkMessage(mavlink_message_t* message)
{
	mavlinkAbstraction->sendMAVLinkMessage(message);
}

void C2Handler::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Forward the received message to the current request
	if (request)
	{
		request->OnMAVLinkMessageReceived(message);
	}

	// Handle the message
	switch (message->msgid)
	{
		case MAVLINK_MSG_ID_HEARTBEAT:
		{
			// Log
			*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling received MAVLink message of type: Heartbeat" << logger->EndLogging();

			// Just send a heartbeat back
			mavlink_message_t* messageToSend = new mavlink_message_t;

			mavlink_msg_heartbeat_pack(ownSystemID, COMPONENT_ID, messageToSend, MAV_TYPE_GCS, MAV_AUTOPILOT_GENERIC, 0, 0, MAV_STATE_ACTIVE);
			SendMAVLinkMessage(messageToSend);

			break;
		}

		case MAVLINK_MSG_ID_STATUSTEXT:
		{
			mavlink_statustext_t decodedMessage;

			mavlink_msg_statustext_decode(message, &decodedMessage);

			// Log
			logger->StartLogging(Logger_Mode::INFO);
			*logger << "Handling received MAVLink message of type: Status text" << Logger::Endl;
			*logger << Logger::Indent << "Status: " << decodedMessage.text;
			logger->EndLogging();

			break;
		}
	}
}

void C2Handler::reset()
{
	isStarted = false;
	request = nullptr;
}