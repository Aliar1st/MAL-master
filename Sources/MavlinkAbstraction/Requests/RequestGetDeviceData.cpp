#include "RequestGetDeviceData.h"

#include <cassert>
#include "MavlinkAbstraction/Logger/Logger.h"
#include "MavlinkAbstraction/MavlinkAbstraction.h"

RequestGetDeviceData::RequestGetDeviceData(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, const std::vector<ResponseGetDeviceDataBase*>& responses)
	: RequestCommand(mavlinkAbstraction, c2Handler, logger)
{
	// Setup request objects
	requests.reserve(responses.size());
	for (ResponseGetDeviceDataBase* response : responses)
	{
		if (response)
		{
			requests.push_back(response->getRequest(logger));
		}
	}

	responseReceived = new bool[requests.size()];
}

RequestGetDeviceData::~RequestGetDeviceData()
{
	delete[] responseReceived;

	// Delete request objects
	for (RequestGetDeviceDataBase* request : requests)
	{
		delete request;
	}
}

void RequestGetDeviceData::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	// Abort if no requests shall be made
	if (requests.empty())
	{
		return;
	}

	// Setup
	memset(responseReceived, false, requests.size() * sizeof(bool));
	allResponsesReceived = false;

	// Send the request commands until responses were received
	while (!allResponsesReceived && mavlinkAbstraction->IsStarted())
	{
		// Request the missing responses
		for (size_t i = 0; i < requests.size(); i++)
		{
			if (!responseReceived[i])
			{
				bool success = executeCommand(uniqueLock, ownSystemID, ownComponentID, targetSystemID, i);

				requests[i]->SetResponseSuccess(success);

				if (!success)
				{
					// If the request was not successfully executed, we cannot request the corresponding data
					// In this case, mark the request as received
					receivedResponse(i);
				}
			}
		}

		// Wait until all messages are received or a timeout occurred
		std::cv_status acknowledgementWaitResult = std::cv_status::no_timeout;

		while (!allResponsesReceived && acknowledgementWaitResult == std::cv_status::no_timeout)
		{
			acknowledgementWaitResult = allResponsesReceivedCondition.wait_for(uniqueLock, std::chrono::milliseconds(REQUESTED_MESSAGES_TIMEOUT));
		}
	}
}

void RequestGetDeviceData::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Handle acknowledgments
	RequestCommand::OnMAVLinkMessageReceived(message);

	// Determine the request corresponding to the MAVLink message
	for (size_t i = 0; i < requests.size(); i++)
	{
		if (!responseReceived[i] && message->msgid == requests[i]->GetRequestedMessageID())
		{
			// Handle the request
			requests[i]->OnResponseReceived(message);

			receivedResponse(i);
		}
	}
}

const char* RequestGetDeviceData::GetName()
{
	return "GetDeviceData";
}

void RequestGetDeviceData::getCommandValues(size_t index, uint16_t* commandID, float params[7])
{
	assert(index < requests.size());

	*commandID = MAV_CMD_REQUEST_MESSAGE;                   // Command: Request message
	params[0] = requests[index]->GetRequestedMessageID();   // MAVLink message to request
	params[1] = 0;                                          // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[2] = 0;                                          // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[3] = 0;                                          // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[4] = 0;                                          // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[5] = 0;                                          // Index ID (Use of tis parameter (if any), must be defined in he requested messages)
	params[6] = 0;                                          // Target address of requested messages (0: Flight-stack, 1: Address of requestor, 2: broadcast)
}

void RequestGetDeviceData::receivedResponse(size_t index)
{
	// Mark as received
	responseReceived[index] = true;

	// Check if all responses received
	for (size_t i = 0; i < requests.size(); i++)
	{
		if (!responseReceived[i])
		{
			return;
		}
	}

	// All requested messages were received
	allResponsesReceived = true;
	allResponsesReceivedCondition.notify_one();
}