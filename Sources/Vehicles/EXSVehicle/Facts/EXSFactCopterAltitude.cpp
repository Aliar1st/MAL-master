#include "EXSFactCopterAltitude.h"

EXSFactCopterAltitude::EXSFactCopterAltitude()
	: EXSFact("CopterAltitude", "VehicleStatus", nullptr)
{
	// For now every altitude is "Level1"
	SetValue("Level1");
}