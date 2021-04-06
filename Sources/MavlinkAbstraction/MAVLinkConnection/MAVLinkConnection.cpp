#include "MAVLinkConnection.h"

#include <cassert>
#include "IPC/Semaphore.h"
#include "MavlinkAbstraction/Logger/Logger.h"

MAVLinkConnection::MAVLinkConnection(MavlinkAbstraction* mavlinkAbstraction, Logger* logger)
	: mavlinkAbstraction(mavlinkAbstraction),
	  logger(logger),
	  readingThread(this, logger, &handlingThread),
	  handlingThread(this, logger),
	  writingThread(this, logger)
{
	reset();
}

MAVLinkConnection::~MAVLinkConnection()
{
	assert(!isConnected); // The MAVLink connection is still connected but should be disconnected by now. Use Disconnect() in the subclass' destructor
}

bool MAVLinkConnection::Connect(MavlinkAbstractionArguments& arguments, unsigned int timeout)
{
	// Check if connected already
	if (isConnected)
	{
		return true;
	}

	// Wait until the threads are fully blocked
	readingThread.BlockUntilIsBlocked();
	handlingThread.BlockUntilIsBlocked();
	writingThread.BlockUntilIsBlocked();

	isConnected = true;

	// Create the semaphore that notifies about the first MAVLink message
	firstMessageReceivedSemaphore.Close();
	firstMessageReceivedSemaphore.Create(NULL, 0);

	// Open the connection
	if (openConnection(arguments, timeout))
	{
		// Connection is opened
		targetSystemID = arguments.targetSystemID;
		firstMessageReceived = false;

		// Unblock the threads
		writingThread.InitializeData(getConnection());
		readingThread.InitializeData(getConnection());

		writingThread.Unblock();
		handlingThread.Unblock();
		readingThread.Unblock();

		// Wait for a message
		firstMessageReceivedSemaphore.DownExtended(timeout == MavlinkAbstraction::NO_TIMEOUT ? Semaphore::NO_TIMEOUT : static_cast<time_interval_t>(timeout));

		// Check if a message was received in time
		if (firstMessageReceived)
		{
			*logger << logger->StartLogging(Logger_Mode::STATUS) << "First message received" << logger->EndLogging();

			return true;
		}

		*logger << logger->StartLogging(Logger_Mode::STATUS) << "No message was received in time" << logger->EndLogging();
	}

	// Disconnect as there was an error/timeout
	Disconnect();

	return false;
}

void MAVLinkConnection::Disconnect()
{
	// Check if connected
	if (!isConnected)
	{
		return;
	}

	// Block the threads
	readingThread.Block();
	handlingThread.Block();
	writingThread.Block();

	// Disconnect from the MAVLink device
	disconnect();

	reset();
}

void MAVLinkConnection::OnMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Check if the received message's system id is the target's system ID (We only want to handle those messages)
	if (message->sysid != targetSystemID)
	{
		return;
	}

	// Notify about received message (setup connection if necessary
	onMessageReceived();

	// Check if the received message is the first one
	if (!firstMessageReceived)
	{
		firstMessageReceived = true;

		// Notify about the message
		firstMessageReceivedSemaphore.Up();
	}

	// Forward the message
	mavlinkAbstraction->onMAVLinkMessageReceived(message);
}

void MAVLinkConnection::SendMAVLinkMessage(mavlink_message_t* message)
{
	writingThread.PushToQueue(message);
}

void MAVLinkConnection::OnError()
{
	// We cannot do much here so just stop the Mavlink Abstraction
	mavlinkAbstraction->Stop();
}

bool MAVLinkConnection::openConnection(MavlinkAbstractionArguments& arguments, unsigned int timeout)
{
	bool connectionOpened = connect(arguments);

	// Connect until the connection is established or a timeout occurs
	while (!connectionOpened && timeout != 0)
	{
		// Calculate the delay until retrying
		unsigned int retryDelay = (timeout == MavlinkAbstraction::NO_TIMEOUT || timeout >= CONNECT_RETRY_INTERVAL ? CONNECT_RETRY_INTERVAL : timeout);

		*logger << logger->StartLogging(Logger_Mode::INFO) << "Trying again in " << retryDelay << "ms" << logger->EndLogging();
		std::this_thread::sleep_for(std::chrono::milliseconds(retryDelay));

		if (timeout != MavlinkAbstraction::NO_TIMEOUT)
		{
			timeout -= retryDelay;
		}

		// Retry connecting
		connectionOpened = connect(arguments);
	}

	return connectionOpened;
}

void MAVLinkConnection::reset()
{
	isConnected = false;
	firstMessageReceived = false;
}