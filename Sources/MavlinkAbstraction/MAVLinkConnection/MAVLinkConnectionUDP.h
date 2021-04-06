#pragma once

#include "IPC/UDPSocket.h"
#include "MAVLinkConnection.h"

/**
 * @brief	A class that handles the connection to the MAVLink device via a UDP socket.
 */
class MAVLinkConnectionUDP : public MAVLinkConnection
{
private:

	UDPSocket udpSocket;

protected:

	virtual bool connect(MavlinkAbstractionArguments& arguments) override;

	virtual void disconnect() override;

	virtual void onMessageReceived() override;

	virtual Connection* getConnection() override;

public:

	/**
	 * @brief	The constructor of MAVLinkConnectionUDP
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this connection belongs to
	 * @param[in]	logger				Logger
	 */
	MAVLinkConnectionUDP(MavlinkAbstraction* mavlinkAbstraction, Logger* logger);

	/**
	 * @brief	The destructor of MAVLinkConnectionUDP.
	 */
	virtual ~MAVLinkConnectionUDP();
};