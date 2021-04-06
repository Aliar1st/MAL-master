#pragma once

#include <stdint.h>
#include "Arguments.h"
#include "MavlinkAbstraction/Logger/Logger_Mode.h"
#include "MavlinkAbstraction/MAVLinkConnection/MAVLink_Connection_Type.h"

/**
 * @brief	Structure holding program arguments of the Mavlink Abstraction
 */
struct MavlinkAbstractionArguments : public Arguments
{
	// Connection
	MAVLink_Connection_Type mavlinkConnectionType = MAVLink_Connection_Type::SERIAL;
	char* serialPortName = nullptr;
	unsigned int socketPort = 0;
	unsigned int baudRate = 0;

	// MAVLink IDs
	uint8_t ownSystemID = 17;
	uint8_t targetSystemID = 1;

	// Logger
	Logger_Mode loggerMode = Logger_Mode::NONE;

	~MavlinkAbstractionArguments();

	/**
	 * @brief	Sets the serial port name by copying the given name.
	 *
	 * @param[in]	serialPortName	The new name
	 */
	void SetSerialPortName(const char* serialPortName);
};