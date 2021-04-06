#include "Request.h"

Request::Request(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger)
	: mavlinkAbstraction(mavlinkAbstraction),
	  c2Handler(c2Handler),
	  logger(logger)
{
}

Request::~Request()
{
}

void Request::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Forward the received message
	if (subRequest)
	{
		subRequest->OnMAVLinkMessageReceived(message);
	}
}

void Request::executeSubRequest(Request* subRequest, std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	this->subRequest = subRequest;

	// Execute the request
	subRequest->Execute(uniqueLock, ownSystemID, ownComponentID, targetSystemID);

	this->subRequest = nullptr;
	delete subRequest;
}