#include "RequestGetDeviceDataLocalPositionNED.h"

#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetDeviceDataLocalPositionNED::RequestGetDeviceDataLocalPositionNED(Logger* logger, ResponseGetDeviceDataLocalPositionNED* response)
	: RequestGetDeviceDataBase(logger),
	  response(response)
{
}

RequestGetDeviceDataLocalPositionNED::~RequestGetDeviceDataLocalPositionNED()
{
}

uint16_t RequestGetDeviceDataLocalPositionNED::GetRequestedMessageID()
{
	return MAVLINK_MSG_ID_LOCAL_POSITION_NED;
}

void RequestGetDeviceDataLocalPositionNED::SetResponseSuccess(bool success)
{
	response->success = success;
}

void RequestGetDeviceDataLocalPositionNED::OnResponseReceived(mavlink_message_t* message)
{
	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling device data: Local Position NED" << logger->EndLogging();

	// Decode message
	mavlink_local_position_ned_t decodedMessage;

	mavlink_msg_local_position_ned_decode(message, &decodedMessage);

	// Set values
	response->deviceData.timeSinceBoot = decodedMessage.time_boot_ms;
	response->deviceData.x = decodedMessage.x;
	response->deviceData.y = decodedMessage.y;
	response->deviceData.z = decodedMessage.z;
	response->deviceData.xSpeed = decodedMessage.vx;
	response->deviceData.ySpeed = decodedMessage.vy;
	response->deviceData.zSpeed = decodedMessage.vz;
}