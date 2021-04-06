#include "RequestGetDeviceDataHeartbeat.h"

#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetDeviceDataHeartbeat::RequestGetDeviceDataHeartbeat(Logger* logger, ResponseGetDeviceDataHeartbeat* response)
	: RequestGetDeviceDataBase(logger),
	  response(response)
{
}

RequestGetDeviceDataHeartbeat::~RequestGetDeviceDataHeartbeat()
{
}

uint16_t RequestGetDeviceDataHeartbeat::GetRequestedMessageID()
{
	return MAVLINK_MSG_ID_HEARTBEAT;
}

void RequestGetDeviceDataHeartbeat::SetResponseSuccess(bool success)
{
	response->success = success;
}

void RequestGetDeviceDataHeartbeat::OnResponseReceived(mavlink_message_t* message)
{
	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling device data: Heartbeat" << logger->EndLogging();

	// Decode message
	mavlink_heartbeat_t decodedMessage;

	mavlink_msg_heartbeat_decode(message, &decodedMessage);

	// Set values
	response->deviceData.copterMode = decodedMessage.custom_mode;
	response->deviceData.modeFlags = decodedMessage.base_mode;
}