#pragma once

#include "IPC/SerialPort.h"
#include "MAVLinkConnection.h"

/**
 * @brief	A class that handles the connection to the MAVLink device via a serial port.
 */
class MAVLinkConnectionSerial : public MAVLinkConnection
{
private:

	SerialPort serialPort;

protected:

	virtual bool connect(MavlinkAbstractionArguments& arguments) override;

	virtual void disconnect() override;

	virtual void onMessageReceived() override;

	virtual Connection* getConnection() override;

public:

	/**
	 * @brief	The constructor of MAVLinkConnectionSerial
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this connection belongs to
	 * @param[in]	logger				Logger
	 */
	MAVLinkConnectionSerial(MavlinkAbstraction* mavlinkAbstraction, Logger* logger);

	/**
	 * @brief	The destructor of MAVLinkConnectionSerial.
	 */
	virtual ~MAVLinkConnectionSerial();
};