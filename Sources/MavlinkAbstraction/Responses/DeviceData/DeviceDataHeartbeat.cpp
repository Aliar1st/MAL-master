#include "DeviceDataHeartbeat.h"

#include "MavlinkAbstraction/MAVLink.h"

bool DeviceDataHeartbeat::IsModeAltHold()
{
	return copterMode == COPTER_MODE_ALT_HOLD;
}

bool DeviceDataHeartbeat::IsModeGuided()
{
	return copterMode == COPTER_MODE_GUIDED;
}

bool DeviceDataHeartbeat::IsModeLand()
{
	return copterMode == COPTER_MODE_LAND;
}

bool DeviceDataHeartbeat::IsModePosHold()
{
	return copterMode == COPTER_MODE_POSHOLD;
}

bool DeviceDataHeartbeat::IsModeRTL()
{
	return copterMode == COPTER_MODE_RTL;
}

bool DeviceDataHeartbeat::IsModeStabilize()
{
	return copterMode == COPTER_MODE_STABILIZE;
}

bool DeviceDataHeartbeat::IsArmed()
{
	return modeFlags & MAV_MODE_FLAG_SAFETY_ARMED;
}

bool DeviceDataHeartbeat::IsDisarmed()
{
	return !IsArmed();
}