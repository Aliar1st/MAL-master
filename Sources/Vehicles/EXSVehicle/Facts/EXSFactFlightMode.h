#pragma once

#include "EXSFact.h"
#include "MavlinkAbstraction/Responses/DeviceData/DeviceDataHeartbeat.h"

/**
 * @brief	Fact of the EXS representing "FlightMode"
 */
class EXSFactFlightMode : public EXSFact
{
public:

	/**
	 * @brief	The constructor of EXSFactFlightMode.
	 *
	 * @param[in]	deviceData	The heartbeat device data
	 */
	EXSFactFlightMode(DeviceDataHeartbeat& deviceData);
};