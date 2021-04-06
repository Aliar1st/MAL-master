#pragma once

#include "EXSFact.h"
#include "MavlinkAbstraction/Responses/DeviceData/DeviceDataHeartbeat.h"

/**
 * @brief	Fact of the EXS representing "CopterState"
 */
class EXSFactCopterState : public EXSFact
{
public:

	/**
	 * @brief	The constructor of EXSFactCopterState.
	 *
	 * @param[in]	deviceData	The heartbeat device data
	 */
	EXSFactCopterState(DeviceDataHeartbeat& deviceData);
};