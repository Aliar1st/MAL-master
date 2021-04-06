#include "EXSFactLandedState.h"

EXSFactLandedState::EXSFactLandedState(DeviceDataExtendedSystemState& deviceData)
	: EXSFact("LandedState", "VehicleStatus", nullptr)
{
	if (deviceData.IsLandedStateOnGround())
	{
		SetValue("OnGround");
	}
	else if (deviceData.IsLandedStateTakeoff())
	{
		SetValue("TakingOff");
	}
	else if (deviceData.IsLandedStateInAir())
	{
		SetValue("InAir");
	}
	else if (deviceData.IsLandedStateLanding())
	{
		SetValue("Landing");
	}
	else
	{
		SetValue("Unknown");
	}
}