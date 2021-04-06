#pragma once

#include <stdint.h>

/**
 * @brief	Structure holding all GPS global origin data of the MAVLink device
 */
struct DeviceDataGPSGlobalOrigin
{
	//                                  Field Name      | Type          | Units                 | Value                 | Description
	// GPS_GLOBAL_ORIGIN            --------------------+---------------+-----------------------+-----------------------+-------------------------------------------------------------------------------------
	int32_t latitude;               //   latitude       | int32_t       | degE7                 |                       | Latitude (WGS84)
	int32_t longitude;              //   longitude      | int32_t       | degE7                 |                       | Latitude (WGS84)
	int32_t altitude;               //   altitude       | int32_t       | mm                    |                       | Altitude (MSL). Positive for up.
};