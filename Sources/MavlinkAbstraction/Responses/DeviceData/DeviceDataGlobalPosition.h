#pragma once

#include <stdint.h>

/**
 * @brief	Structure holding all global position data of the MAVLink device
 */
struct DeviceDataGlobalPosition
{
	//                                  Field Name      | Type          | Units                 | Value                 | Description
	// GLOBAL_POSITION_INT          --------------------+---------------+-----------------------+-----------------------+-------------------------------------------------------------------------------------
	uint32_t timeSinceBoot;         //   time_boot_ms   | uint32_t      | ms                    |                       | Timestamp (time since system boot).
	int32_t latitude;               //   lat            | int32_t       | degE7                 |                       | Latitude, expressed
	int32_t longitude;              //   lon            | int32_t       | degE7                 |                       | Longitude, expressed
	int32_t altitude;               //   alt            | int32_t       | mm                    |                       | Altitude (MSL). Note that virtually all GPS modules provide both WGS84 and MSL.
	int32_t relativeAltitude;       //   relative_alt   | int32_t       | mm                    |                       | Altitude above ground
	int16_t xSpeed;                 //   vx             | int16_t       | cm/s                  |                       | Ground X Speed (Latitude, positive north)
	int16_t ySpeed;                 //   vy             | int16_t       | cm/s                  |                       | Ground Y Speed (Longitude, positive east)
	int16_t zSpeed;                 //   vz             | int16_t       | cm/s                  |                       | Ground Z Speed (Altitude, positive down)
	uint16_t heading;               //   hdg            | uint16_t      | cdeg                  |                       | Vehicle heading (yaw angle), 0.0..359.99 degrees. If unknown, set to: UINT16_MAX
};