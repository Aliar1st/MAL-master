#pragma once

#include <stdint.h>

/**
 * @brief	Structure holding all attitude data of the MAVLink device
 */
struct DeviceDataAttitude
{
	//                                  Field Name      | Type          | Units                 | Value                 | Description
	// ATTITUDE                     --------------------+---------------+-----------------------+-----------------------+-------------------------------------------------------------------------------------
	uint32_t timeSinceBoot;         //   time_boot_ms   | uint32_t      | ms                    |                       | Timestamp (time since system boot).
	float roll;                     //   roll           | float         | rad                   |                       | Roll angle (-pi..+pi)
	float pitch;                    //   pitch          | float         | rad                   |                       | Pitch angle (-pi..+pi)
	float yaw;                      //   yaw            | float         | rad                   |                       | Yaw angle (-pi..+pi)
	float rollSpeed;                //   rollspeed      | float         | rad/s                 |                       | Roll angular speed
	float pitchSpeed;               //   pitchspeed     | float         | rad/s                 |                       | Pitch angular speed
	float yawSpeed;                 //   yawspeed       | float         | rad/s                 |                       | Yaw angular speed
};