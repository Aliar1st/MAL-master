#pragma once

#include <stdint.h>

/**
 * @brief	Structure holding all local position data in the NED frame of the MAVLink device
 */
struct DeviceDataLocalPositionNED
{
	//                                  Field Name      | Type          | Units                 | Value                 | Description
	// LOCAL_POSITION_NED           --------------------+---------------+-----------------------+-----------------------+-------------------------------------------------------------------------------------
	uint32_t timeSinceBoot;         //   time_boot_ms   | uint32_t      | ms                    |                       | Timestamp (time since system boot).
	float x;                        //   x              | float         | m                     |                       | X Position
	float y;                        //   y              | float         | m                     |                       | Y Position
	float z;                        //   z              | float         | m                     |                       | Z Position
	float xSpeed;                   //   vx             | float         | m/s                   |                       | X Speed
	float ySpeed;                   //   vy             | float         | m/s                   |                       | Y Speed
	float zSpeed;                   //   vz             | float         | m/s                   |                       | Z Speed
};