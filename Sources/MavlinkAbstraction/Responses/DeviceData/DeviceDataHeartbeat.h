#pragma once

#include <stdint.h>

/**
 * @brief	Structure holding all heartbeat data of the MAVLink device
 */
struct DeviceDataHeartbeat
{
	//                                  Field Name      | Type          | Units                 | Value                 | Description
	// HEARTBEAT                    --------------------+---------------+-----------------------+-----------------------+-------------------------------------------------------------------------------------
	uint32_t copterMode;            //   custom_mode    | uint32_t      |                       |                       | A bitfield for use for autopilot-specific flags
	uint8_t modeFlags;              //   base_mode      | uint8_t       |                       | MAV_MODE_FLAG         | System mode bitmap

	bool IsModeAltHold();

	bool IsModeGuided();

	bool IsModeLand();

	bool IsModePosHold();

	bool IsModeRTL();

	bool IsModeStabilize();

	bool IsArmed();

	bool IsDisarmed();
};