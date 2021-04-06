#pragma once

#include "MavlinkAbstraction/MAVLink.h"
#include "Threading/BlockingQueueThread.h"

class Logger;
class MAVLinkConnection;

/**
 * @brief	Wraps a thread that handles received MAVLink messages.
 *			This thread exists because reading from the connection should be done as often as possible to prevent losing data.
 *			Therefore, the reading thread passes the message to this thread and this thread will handle it so the reading thread can continue reading.
 */
class MAVLinkConnectionHandlingThread : public BlockingQueueThread<mavlink_message_t>
{
private:

	MAVLinkConnection* mavlinkConnection;
	Logger* logger;

protected:

	virtual bool elementPopped(mavlink_message_t* message) override;

public:

	/**
	 * @brief	The constructor of MAVLinkConnectionHandlingThread.
	 *
	 * @param[in]	mavlinkConnection	MAVLink connection this thread belongs to
	 * @param[in]	logger				Logger
	 */
	MAVLinkConnectionHandlingThread(MAVLinkConnection* mavlinkConnection, Logger* logger);

	/**
	 * @brief	The destructor of MAVLinkConnectionHandlingThread.
	 */
	~MAVLinkConnectionHandlingThread();

	virtual const char* GetName() override;
};