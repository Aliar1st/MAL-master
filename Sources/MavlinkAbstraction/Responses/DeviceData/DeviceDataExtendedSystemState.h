#pragma once

#include <stdint.h>

/**
 * @brief	Structure holding all extended system state data of the MAVLink device
 */
struct DeviceDataExtendedSystemState
{
	//                                  Field Name      | Type          | Units                 | Value                 | Description
	// EXTENDED_SYS_STATE           --------------------+---------------+-----------------------+-----------------------+-------------------------------------------------------------------------------------
	uint8_t landedState;            //   landed_state   | uint8_t       |                       | MAV_LANDED_STATE      | The landed state. Is set to MAV_LANDED_STATE_UNDEFINED if landed state is unknown

	bool IsLandedStateUndefined();

	bool IsLandedStateOnGround();

	bool IsLandedStateInAir();

	bool IsLandedStateTakeoff();

	bool IsLandedStateLanding();
};