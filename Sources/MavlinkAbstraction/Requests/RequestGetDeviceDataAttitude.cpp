#include "RequestGetDeviceDataAttitude.h"

#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetDeviceDataAttitude::RequestGetDeviceDataAttitude(Logger* logger, ResponseGetDeviceDataAttitude* response)
	: RequestGetDeviceDataBase(logger),
	  response(response)
{
}

RequestGetDeviceDataAttitude::~RequestGetDeviceDataAttitude()
{
}

uint16_t RequestGetDeviceDataAttitude::GetRequestedMessageID()
{
	return MAVLINK_MSG_ID_ATTITUDE;
}

void RequestGetDeviceDataAttitude::SetResponseSuccess(bool success)
{
	response->success = success;
}

void RequestGetDeviceDataAttitude::OnResponseReceived(mavlink_message_t* message)
{
	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling device data: Attitude" << logger->EndLogging();

	// Decode message
	mavlink_attitude_t decodedMessage;

	mavlink_msg_attitude_decode(message, &decodedMessage);

	// Set values
	response->deviceData.timeSinceBoot = decodedMessage.time_boot_ms;
	response->deviceData.roll = decodedMessage.roll;
	response->deviceData.pitch = decodedMessage.pitch;
	response->deviceData.yaw = decodedMessage.yaw;
	response->deviceData.rollSpeed = decodedMessage.rollspeed;
	response->deviceData.pitchSpeed = decodedMessage.pitchspeed;
	response->deviceData.yawSpeed = decodedMessage.yawspeed;
}