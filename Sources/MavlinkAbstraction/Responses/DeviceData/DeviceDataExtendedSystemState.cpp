#include "DeviceDataExtendedSystemState.h"

#include "MavlinkAbstraction/MAVLink.h"

bool DeviceDataExtendedSystemState::IsLandedStateUndefined()
{
	return landedState == MAV_LANDED_STATE_UNDEFINED;
}

bool DeviceDataExtendedSystemState::IsLandedStateOnGround()
{
	return landedState == MAV_LANDED_STATE_ON_GROUND;
}

bool DeviceDataExtendedSystemState::IsLandedStateInAir()
{
	return landedState == MAV_LANDED_STATE_IN_AIR;
}

bool DeviceDataExtendedSystemState::IsLandedStateTakeoff()
{
	return landedState == MAV_LANDED_STATE_TAKEOFF;
}

bool DeviceDataExtendedSystemState::IsLandedStateLanding()
{
	return landedState == MAV_LANDED_STATE_LANDING;
}