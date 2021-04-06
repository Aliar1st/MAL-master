#include "EXSVehicleCommandTakeOff.h"

EXSVehicleCommandTakeOff::EXSVehicleCommandTakeOff()
	: VehicleCommandTakeOff(TAKE_OFF_ALTITUDE)
{
}

const char* EXSVehicleCommandTakeOff::GetName()
{
	return "TakeOffRequested";
}