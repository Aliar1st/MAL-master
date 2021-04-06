#include "MAVLinkConnectionHandlingThread.h"

#include "MavlinkAbstraction/Logger/Logger.h"
#include "MavlinkAbstraction/MAVLinkConnection/MAVLinkConnection.h"

MAVLinkConnectionHandlingThread::MAVLinkConnectionHandlingThread(MAVLinkConnection* mavlinkConnection, Logger* logger)
	: mavlinkConnection(mavlinkConnection),
	  logger(logger)
{
	// Start the thread
	start();
}

MAVLinkConnectionHandlingThread::~MAVLinkConnectionHandlingThread()
{
	join();
}

const char* MAVLinkConnectionHandlingThread::GetName()
{
	return "MAVLinkConnectionHandlingThread";
}

bool MAVLinkConnectionHandlingThread::elementPopped(mavlink_message_t* message)
{
	// Handle the message
	mavlinkConnection->OnMAVLinkMessageReceived(message);

	return true;
}