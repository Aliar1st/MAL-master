#pragma once

#include "IPC/Connection.h"
#include "MavlinkAbstraction/MAVLink.h"
#include "Threading/BlockingQueueThread.h"

class Logger;
class MAVLinkConnection;

/**
 * @brief	Wraps a thread that writes to the MAVLink connection.
 *			This thread exists because writing to the connection might block until it can be written to.
 */
class MAVLinkConnectionWritingThread : public BlockingQueueThread<mavlink_message_t>
{
private:

	Connection* connection;
	ConnectionAsynchronousOperation* connectionAsynchronousOperation;
	Connection_Return connectionReturnValue;

	MAVLinkConnection* mavlinkConnection;
	Logger* logger;

	/**
	 * @brief	Logs a MAVLink message that has been sent
	 *
	 * @param[in]	message		MAVLink message to log
	 */
	void logSentMessage(uint8_t* messageBytestream, uint16_t messageLength);

protected:

	virtual void blocked() override;

	virtual void block() override;

	virtual void unblock() override;

	virtual void unblockedRoutine() override;

	virtual bool elementPopped(mavlink_message_t* message) override;

public:

	/**
	 * @brief	The constructor of MAVLinkConnectionWritingThread.
	 *
	 * @param[in]	mavlinkConnection	MAVLink connection this thread belongs to
	 * @param[in]	logger				Logger
	 */
	MAVLinkConnectionWritingThread(MAVLinkConnection* mavlinkConnection, Logger* logger);

	/**
	 * @brief	The destructor of MAVLinkConnectionWritingThread.
	 */
	~MAVLinkConnectionWritingThread();

	/**
	 * @brief	Initializes all data for this thread.
	 *
	 * @param[in]	connection	The connection to the MAVLink device
	 */
	void InitializeData(Connection* connection);

	virtual const char* GetName() override;
};