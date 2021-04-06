#include "MAVLinkConnectionWritingThread.h"

#include "MavlinkAbstraction/Logger/Logger.h"
#include "MavlinkAbstraction/MAVLinkConnection/MAVLinkConnection.h"

MAVLinkConnectionWritingThread::MAVLinkConnectionWritingThread(MAVLinkConnection* mavlinkConnection, Logger* logger)
	: mavlinkConnection(mavlinkConnection),
	  logger(logger)
{
	// Start the thread
	start();
}

MAVLinkConnectionWritingThread::~MAVLinkConnectionWritingThread()
{
	join();
}

void MAVLinkConnectionWritingThread::InitializeData(Connection* connection)
{
	this->connection = connection;
}

const char* MAVLinkConnectionWritingThread::GetName()
{
	return "MAVLinkConnectionWritingThread";
}

void MAVLinkConnectionWritingThread::blocked()
{
	BlockingQueueThread::blocked();

	delete connectionAsynchronousOperation;
	connectionAsynchronousOperation = nullptr;
}

void MAVLinkConnectionWritingThread::block()
{
	BlockingQueueThread::block();

	connection->CancelOperation(connectionAsynchronousOperation);
}

void MAVLinkConnectionWritingThread::unblock()
{
	BlockingQueueThread::unblock();

	connectionAsynchronousOperation = connection->CreateAsynchronousOperation();
	connectionReturnValue = CONNECTION_RETURN_SUCCESS;
}

void MAVLinkConnectionWritingThread::unblockedRoutine()
{
	BlockingQueueThread::unblockedRoutine();

	// Check if an error occurred
	if (connectionReturnValue != CONNECTION_RETURN_SUCCESS && connectionReturnValue != CONNECTION_RETURN_ERROR_OPERATION_ABORTED)
	{
		// Log the error
		logger->StartLogging(Logger_Mode::ERRORS);
		*logger << "Error while writing to the MAVLink connection" << Logger::Endl;
		*logger << Logger::Indent << "Error code: " << connectionReturnValue;
		logger->EndLogging();

		mavlinkConnection->OnError();
	}
}

bool MAVLinkConnectionWritingThread::elementPopped(mavlink_message_t* message)
{
	// Convert the message to a byte stream
	uint8_t messageBytestream[MAVLINK_MAX_PACKET_LEN];
	uint16_t messageLength = mavlink_msg_to_send_buffer(messageBytestream, message);

	// Log
	logSentMessage(messageBytestream, messageLength);

	// Send the message
	connectionReturnValue = connection->WriteArray(messageBytestream, messageLength, NULL, connectionAsynchronousOperation);
	if (shallExecute() && connectionReturnValue == CONNECTION_RETURN_SUCCESS)
	{
		// Wait
		connectionReturnValue = connection->WaitForOperation(connectionAsynchronousOperation, NULL);
	}

	return connectionReturnValue == CONNECTION_RETURN_SUCCESS;
}

void MAVLinkConnectionWritingThread::logSentMessage(uint8_t* messageBytestream, uint16_t messageLength)
{
	logger->StartLogging(Logger_Mode::INFO);

	// Prevent as much overhead as possible
	if (logger->IsLogging())
	{
		*logger << "Sent MAVLink message: " << Logger::Endl << Logger::Indent;

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