#include "RequestGetDeviceDataGPSGlobalOrigin.h"

#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetDeviceDataGPSGlobalOrigin::RequestGetDeviceDataGPSGlobalOrigin(Logger* logger, ResponseGetDeviceDataGPSGlobalOrigin* response)
	: RequestGetDeviceDataBase(logger),
	  response(response)
{
}

RequestGetDeviceDataGPSGlobalOrigin::~RequestGetDeviceDataGPSGlobalOrigin()
{
}

uint16_t RequestGetDeviceDataGPSGlobalOrigin::GetRequestedMessageID()
{
	return MAVLINK_MSG_ID_HOME_POSITION;
}

void RequestGetDeviceDataGPSGlobalOrigin::SetResponseSuccess(bool success)
{
	response->success = success;
}

void RequestGetDeviceDataGPSGlobalOrigin::OnResponseReceived(mavlink_message_t* message)
{
	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling device data: Home Position" << logger->EndLogging();

	// Decode message
	mavlink_home_position_t decodedMessage;

	mavlink_msg_home_position_decode(message, &decodedMessage);

	// Set values
	response->deviceData.latitude = decodedMessage.latitude;
	response->deviceData.longitude = decodedMessage.longitude;
	response->deviceData.altitude = decodedMessage.altitude;
}