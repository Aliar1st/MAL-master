#pragma once

#include "IPC/Connection.h"
#include "MavlinkAbstraction/MAVLink.h"
#include "Threading/BlockingThread.h"

class Logger;
class MAVLinkConnection;
class MAVLinkConnectionHandlingThread;

/**
 * @brief	Wraps a thread that reads from the MAVLink connection.
 *			This thread exists because reading from the connection blocks until there is data to read.
 */
class MAVLinkConnectionReadingThread : public BlockingThread
{
private:

	Connection* connection;
	ConnectionAsynchronousOperation* connectionAsynchronousOperation;

	MAVLinkConnection* mavlinkConnection;
	Logger* logger;
	MAVLinkConnectionHandlingThread* handlingThread;

	/**
	 * @brief	Logs a MAVLink message that has been received
	 *
	 * @param[in]	message		MAVLink message to log
	 */
	void logReceivedMessage(mavlink_message_t* message);

protected:

	virtual void blocked() override;

	virtual void block() override;

	virtual void unblock() override;

	virtual void unblockedRoutine() override;

public:

	/**
	 * @brief	The constructor of MAVLinkConnectionReadingThread.
	 *
	 * @param[in]	mavlinkConnection	MAVLink connection this thread belongs to
	 * @param[in]	logger				Logger
	 * @param[in]	handlingThread		Thread that handles received MAVLink messages
	 */
	MAVLinkConnectionReadingThread(MAVLinkConnection* mavlinkConnection, Logger* logger, MAVLinkConnectionHandlingThread* handlingThread);

	/**
	 * @brief	The destructor of MAVLinkConnectionReadingThread.
	 */
	~MAVLinkConnectionReadingThread();

	/**
	 * @brief	Initializes all data for this thread.
	 *
	 * @param[in]	connection	The connection to the MAVLink device
	 */
	void InitializeData(Connection* connection);

	virtual const char* GetName() override;
};