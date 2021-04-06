#include "RequestPosHold.h"

RequestPosHold::RequestPosHold(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponsePosHold* response)
	: RequestChangeMode(mavlinkAbstraction, c2Handler, logger, response)
{
}

RequestPosHold::~RequestPosHold()
{
}

const char* RequestPosHold::GetName()
{
	return "PosHold";
}

float RequestPosHold::getMode()
{
	return COPTER_MODE_POSHOLD;
}