#include "RequestAltHold.h"

RequestAltHold::RequestAltHold(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseAltHold* response)
	: RequestChangeMode(mavlinkAbstraction, c2Handler, logger, response)
{
}

RequestAltHold::~RequestAltHold()
{
}

const char* RequestAltHold::GetName()
{
	return "AltHold";
}

float RequestAltHold::getMode()
{
	return COPTER_MODE_ALT_HOLD;
}