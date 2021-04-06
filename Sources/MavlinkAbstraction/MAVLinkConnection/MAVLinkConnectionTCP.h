#pragma once

#include "IPC/TCPSocket.h"
#include "MAVLinkConnection.h"

/**
 * @brief	A class that handles the connection to the MAVLink device via a TCP socket.
 */
class MAVLinkConnectionTCP : public MAVLinkConnection
{
private:

	TCPSocket tcpServerSocket;
	TCPSocket tcpClientSocket;
	TCPSocketAsynchronousOperation* acceptClientAsyncOp = nullptr;

protected:

	virtual bool connect(MavlinkAbstractionArguments& arguments) override;

	virtual void disconnect() override;

	virtual void onMessageReceived() override;

	virtual Connection* getConnection() override;

public:

	/**
	 * @brief	The constructor of MAVLinkConnectionTCP
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this connection belongs to
	 * @param[in]	logger				Logger
	 */
	MAVLinkConnectionTCP(MavlinkAbstraction* mavlinkAbstraction, Logger* logger);

	/**
	 * @brief	The destructor of MAVLinkConnectionTCP.
	 */
	virtual ~MAVLinkConnectionTCP();
};