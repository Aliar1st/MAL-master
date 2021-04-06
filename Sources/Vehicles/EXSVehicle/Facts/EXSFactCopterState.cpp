#include "EXSFactCopterState.h"

EXSFactCopterState::EXSFactCopterState(DeviceDataHeartbeat& deviceData)
	: EXSFact("CopterState", "VehicleStatus", nullptr)
{
	if (deviceData.IsArmed())
	{
		SetValue("Armed");
	}
	else
	{
		SetValue("Disarmed");
	}
}