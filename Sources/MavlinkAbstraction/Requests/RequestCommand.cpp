#include "RequestCommand.h"

#include "MavlinkAbstraction/C2Handler/C2Handler.h"
#include "MavlinkAbstraction/Logger/Logger.h"
#include "MavlinkAbstraction/MavlinkAbstraction.h"

RequestCommand::RequestCommand(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger)
	: Request(mavlinkAbstraction, c2Handler, logger)
{
}

RequestCommand::~RequestCommand()
{
}

bool RequestCommand::executeCommand(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID, size_t index)
{
	acknowledgementReceived = false;

	// Send the MAVLink command until it was acknowledged
	for (uint8_t confirmationNumber = 0; !acknowledgementReceived && mavlinkAbstraction->IsStarted(); confirmationNumber++)
	{
		// Log
		if (confirmationNumber > 0)
		{
			*logger << logger->StartLogging(Logger_Mode::INFO) << "Resending command request: " << GetName() << logger->EndLogging();
		}

		// Get the command values
		uint16_t commandID;
		float params[7];

		getCommandValues(index, &commandID, params);

		// Define the message
		mavlink_message_t* message = new mavlink_message_t;

		mavlink_msg_command_long_pack(
			ownSystemID,                    // Own system ID
			ownComponentID,                 // Own component ID
			message,                        // MAVLink message to pack the data into
			targetSystemID,                 // Target's system ID
			0,                              // Target's component ID (0 means broadcast)
			commandID,                      // Command ID
			confirmationNumber,             // Confirmation
			params[0],                      // Parameter 1
			params[1],                      // Parameter 2
			params[2],                      // Parameter 3
			params[3],                      // Parameter 4
			params[4],                      // Parameter 5
			params[5],                      // Parameter 6
			params[6]                       // Parameter 7
		);

		// Send the message
		c2Handler->SendMAVLinkMessage(message);

		// Wait until the command is acknowledged or a timeout occurred
		std::cv_status acknowledgementWaitResult = std::cv_status::no_timeout;

		while (!acknowledgementReceived && acknowledgementWaitResult == std::cv_status::no_timeout)
		{
			acknowledgementWaitResult = acknowledgementReceivedCondition.wait_for(uniqueLock, std::chrono::milliseconds(ACKNOWLEDGEMENT_TIMEOUT));
		}
	}

	// We got an acknowledgement
	return succeeded;
}

void RequestCommand::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Handle sub requests
	Request::OnMAVLinkMessageReceived(message);

	// Check if acknowledgement message
	if (message->msgid != MAVLINK_MSG_ID_COMMAND_ACK)
	{
		return;
	}

	// Decode the acknowledgement
	mavlink_command_ack_t decodedMessage;

	mavlink_msg_command_ack_decode(message, &decodedMessage);

	// Log
	logger->StartLogging(Logger_Mode::INFO);
	*logger << "Handling received MAVLink message of type: Acknowledgement" << Logger::Endl;
	*logger << Logger::Indent << "Command: " << decodedMessage.command << Logger::Endl;
	*logger << Logger::Indent << "Result: " << static_cast<int>(decodedMessage.result);
	logger->EndLogging();

	// Notify about the received acknowledgement
	succeeded = decodedMessage.result == MAV_RESULT_ACCEPTED;
	acknowledgementReceived = true;
	acknowledgementReceivedCondition.notify_one();
}