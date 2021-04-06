#include "RequestGetDeviceDataGlobalPosition.h"

#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetDeviceDataGlobalPosition::RequestGetDeviceDataGlobalPosition(Logger* logger, ResponseGetDeviceDataGlobalPosition* response)
	: RequestGetDeviceDataBase(logger),
	  response(response)
{
}

RequestGetDeviceDataGlobalPosition::~RequestGetDeviceDataGlobalPosition()
{
}

uint16_t RequestGetDeviceDataGlobalPosition::GetRequestedMessageID()
{
	return MAVLINK_MSG_ID_GLOBAL_POSITION_INT;
}

void RequestGetDeviceDataGlobalPosition::SetResponseSuccess(bool success)
{
	response->success = success;
}

void RequestGetDeviceDataGlobalPosition::OnResponseReceived(mavlink_message_t* message)
{
	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling device data: Global Position Int" << logger->EndLogging();

	// Decode message
	mavlink_global_position_int_t decodedMessage;

	mavlink_msg_global_position_int_decode(message, &decodedMessage);

	// Set values
	response->deviceData.timeSinceBoot = decodedMessage.time_boot_ms;
	response->deviceData.latitude = decodedMessage.lat;
	response->deviceData.longitude = decodedMessage.lon;
	response->deviceData.altitude = decodedMessage.alt;
	response->deviceData.relativeAltitude = decodedMessage.relative_alt;
	response->deviceData.xSpeed = decodedMessage.vx;
	response->deviceData.ySpeed = decodedMessage.vy;
	response->deviceData.zSpeed = decodedMessage.vz;
	response->deviceData.heading = decodedMessage.hdg;
}