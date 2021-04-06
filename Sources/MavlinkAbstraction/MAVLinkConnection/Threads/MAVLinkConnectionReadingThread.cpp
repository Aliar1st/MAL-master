#include "MAVLinkConnectionReadingThread.h"

#include "MAVLinkConnectionHandlingThread.h"
#include "MavlinkAbstraction/Logger/Logger.h"
#include "MavlinkAbstraction/MAVLinkConnection/MAVLinkConnection.h"

MAVLinkConnectionReadingThread::MAVLinkConnectionReadingThread(MAVLinkConnection* mavlinkConnection, Logger* logger, MAVLinkConnectionHandlingThread* handlingThread)
	: mavlinkConnection(mavlinkConnection),
	  logger(logger),
	  handlingThread(handlingThread)
{
	// Start the thread
	start();
}

MAVLinkConnectionReadingThread::~MAVLinkConnectionReadingThread()
{
	join();
}

void MAVLinkConnectionReadingThread::InitializeData(Connection* connection)
{
	this->connection = connection;
}

const char* MAVLinkConnectionReadingThread::GetName()
{
	return "MAVLinkConnectionReadingThread";
}

void MAVLinkConnectionReadingThread::blocked()
{
	delete connectionAsynchronousOperation;
	connectionAsynchronousOperation = nullptr;
}

void MAVLinkConnectionReadingThread::block()
{
	connection->CancelOperation(connectionAsynchronousOperation);
}

void MAVLinkConnectionReadingThread::unblock()
{
	connectionAsynchronousOperation = connection->CreateAsynchronousOperation();
}

void MAVLinkConnectionReadingThread::unblockedRoutine()
{
	mavlink_message_t* message = new mavlink_message_t;
	mavlink_status_t status;

	Connection_Return connectionReturnValue = CONNECTION_RETURN_SUCCESS;

	// While this thread shouldn't block, handle the messages
	while (shallExecute() && connectionReturnValue == CONNECTION_RETURN_SUCCESS)
	{
		// Allocate a buffer to read in
		uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
		transfer_size_t bytesRead;

		// Read from the connection
		connectionReturnValue = connection->ReadArray(buffer, MAVLINK_MAX_PACKET_LEN, NULL, connectionAsynchronousOperation);

		if (shallExecute() && connectionReturnValue == CONNECTION_RETURN_SUCCESS)
		{
			// Wait
			connectionReturnValue = connection->WaitForOperation(connectionAsynchronousOperation, &bytesRead);

			if (shallExecute() && connectionReturnValue == CONNECTION_RETURN_SUCCESS)
			{
				// Parse the mavlink message
				for (transfer_size_t i = 0; i < bytesRead; i++)
				{
					if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], message, &status))
					{
						// The message was successfully parsed and represents a valid message.
						logReceivedMessage(message);

						// Forward the message to the handling thread
						handlingThread->PushToQueue(message);

						// Provide memory for a new message
						message = new mavlink_message_t;
					}
				}
			}
		}
	}

	// Check if an error occurred
	if (connectionReturnValue != CONNECTION_RETURN_SUCCESS && connectionReturnValue != CONNECTION_RETURN_ERROR_OPERATION_ABORTED)
	{
		// Log the error
		logger->StartLogging(Logger_Mode::ERRORS);
		*logger << "Error while reading from the MAVLink connection" << Logger::Endl;
		*logger << Logger::Indent << "Error code: " << connectionReturnValue;
		logger->EndLogging();

		mavlinkConnection->OnError();
	}

	// Delete the last message as it is incomplete
	delete message;
}

void MAVLinkConnectionReadingThread::logReceivedMessage(mavlink_message_t* message)
{
	logger->StartLogging(Logger_Mode::INFO);

	// Prevent as much overhead as possible
	if (logger->IsLogging())
	{
		*logger << "Received MAVLink message: " << Logger::Endl << Logger::Indent;

		// Convert the received message to a byte stream
		uint8_t messageBytestream[MAVLINK_MAX_PACKET_LEN];
		uint16_t messageLength = mavlink_msg_to_send_buffer(messageBytestream, message);

		for (uint16_t i = 0; i < messageLength; i++)
		{
			*logger << static_cast<int>(messageBytestream[i]);
			if (i < messageLength - 1)
			{
				*logger << " ";
			}
		}
	}

	logger->EndLogging();
}