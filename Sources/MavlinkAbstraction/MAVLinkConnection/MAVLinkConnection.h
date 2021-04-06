#pragma once

#include <mutex>
#include "IPC/Semaphore.h"
#include "MavlinkAbstraction/Logger/Logger_Mode.h"
#include "MavlinkAbstraction/MavlinkAbstraction.h"
#include "Threads/MAVLinkConnectionHandlingThread.h"
#include "Threads/MAVLinkConnectionReadingThread.h"
#include "Threads/MAVLinkConnectionWritingThread.h"

/**
 * @brief	An abstract class that handles the connection to the MAVLink device.
 */
class MAVLinkConnection
{
private:

	constexpr static const unsigned int CONNECT_RETRY_INTERVAL = 1000;

	bool isConnected;
	uint8_t targetSystemID;

	bool firstMessageReceived;
	Semaphore firstMessageReceivedSemaphore;

	MavlinkAbstraction* mavlinkAbstraction;

	// Threads
	MAVLinkConnectionReadingThread readingThread;
	MAVLinkConnectionHandlingThread handlingThread;
	MAVLinkConnectionWritingThread writingThread;

	/**
	 * @brief	Opens the connection to a MAVLink device
	 *
	 * @param[in]	arguments	Arguments of the Mavlink Abstraction
	 * @param		timeout		Timeout until the connection must be opened in milliseconds. If MavlinkAbstraction::NO_TIMEOUT, no timeout occurs.
	 *
	 * @return	true if the connection could be opened; False if a timeout occurs
	 */
	bool openConnection(MavlinkAbstractionArguments& arguments, unsigned int timeout);

	/**
	 * @brief	Resets the MAVLink connection to its original state
	 */
	void reset();

protected:

	Logger* logger;

	/**
	 * @brief	Connects to a MAVLink device
	 *
	 * @param[in]	arguments	Arguments of the Mavlink Abstraction
	 *
	 * @return	true if the connection could be established; False otherwise
	 */
	virtual bool connect(MavlinkAbstractionArguments& arguments) = 0;

	/**
	 * @brief	Function that is called when a MAVLink message was received
	 */
	virtual void onMessageReceived() = 0;

	/**
	 * @brief	Disconnects from the MAVLink device
	 *			This function is also called when the connection was opened but no message was received in time to disconnect from the MAVLink connection again.
	 */
	virtual void disconnect() = 0;

	/**
	 * @brief	Returns the connection.
	 *
	 * @return	A pointer to the connection
	 */
	virtual Connection* getConnection() = 0;

public:

	/**
	 * @brief	The constructor of MAVLinkConnection
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this connection belongs to
	 * @param[in]	logger				Logger
	 */
	MAVLinkConnection(MavlinkAbstraction* mavlinkAbstraction, Logger* logger);

	/**
	 * @brief	The destructor of MAVLinkConnection.
	 */
	virtual ~MAVLinkConnection();

	/**
	 * @brief	Connects to a MAVLink device and waits for the first message to be received
	 *
	 * @param[in]	arguments	Arguments of the Mavlink Abstraction
	 * @param		timeout		Timeout until the connection must be opened and until a MAVlink message has been received in milliseconds.
	 *							If MavlinkAbstraction::NO_TIMEOUT, no timeout occurs.
	 *
	 * @return	true if the connection could be established; False if a timeout occurred
	 *			Also returns true if a connection is already established
	 */
	bool Connect(MavlinkAbstractionArguments& arguments, unsigned int timeout);

	/**
	 * @brief	Disconnects from the MAVLink device.
	 */
	void Disconnect();

	/**
	 * @brief	Function to call whenever a MAVLink message is received.
	 *			The received MAVLink message's system ID might not match the target's system ID.
	 *
	 * @param[in]	message		The received message.
	 */
	void OnMAVLinkMessageReceived(mavlink_message_t* message);

	/**
	 * @brief	Sends a MAVLink message to the MAVLink connection
	 *
	 * @param[in]	message		The message to send. This object must be allocated dynamically using new.
	 */
	void SendMAVLinkMessage(mavlink_message_t* message);

	/**
	 * @brief	Function to call whenever an error occurs with the MAVLink connection
	 */
	void OnError();
};