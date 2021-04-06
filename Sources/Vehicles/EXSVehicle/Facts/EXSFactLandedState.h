#pragma once

#include "EXSFact.h"
#include "MavlinkAbstraction/Responses/DeviceData/DeviceDataExtendedSystemState.h"

/**
 * @brief	Fact of the EXS representing "LandedState"
 */
class EXSFactLandedState : public EXSFact
{
public:

	/**
	 * @brief	The constructor of EXSFactLandedState.
	 *
	 * @param[in]	deviceData	The extended system state device data
	 */
	EXSFactLandedState(DeviceDataExtendedSystemState& deviceData);
};