#include "RequestGetDeviceDataExtendedSystemState.h"

#include "MavlinkAbstraction/Logger/Logger.h"

RequestGetDeviceDataExtendedSystemState::RequestGetDeviceDataExtendedSystemState(Logger* logger, ResponseGetDeviceDataExtendedSystemState* response)
	: RequestGetDeviceDataBase(logger),
	  response(response)
{
}

RequestGetDeviceDataExtendedSystemState::~RequestGetDeviceDataExtendedSystemState()
{
}

uint16_t RequestGetDeviceDataExtendedSystemState::GetRequestedMessageID()
{
	return MAVLINK_MSG_ID_EXTENDED_SYS_STATE;
}

void RequestGetDeviceDataExtendedSystemState::SetResponseSuccess(bool success)
{
	response->success = success;
}

void RequestGetDeviceDataExtendedSystemState::OnResponseReceived(mavlink_message_t* message)
{
	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling device data: Extended System State" << logger->EndLogging();

	// Decode message
	mavlink_extended_sys_state_t decodedMessage;

	mavlink_msg_extended_sys_state_decode(message, &decodedMessage);

	// Set values
	response->deviceData.landedState = decodedMessage.landed_state;
}