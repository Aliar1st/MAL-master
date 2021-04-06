#include "EXSFactFlightMode.h"

EXSFactFlightMode::EXSFactFlightMode(DeviceDataHeartbeat& deviceData)
	: EXSFact("FlightMode", "VehicleStatus", nullptr)
{
	if (deviceData.IsModeStabilize())
	{
		SetValue("Stabilize");
	}
	else if (deviceData.IsModeGuided())
	{
		SetValue("Guided");
	}
	else if (deviceData.IsModeLand())
	{
		SetValue("Land");
	}
}