#include "RequestRTL.h"

RequestRTL::RequestRTL(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseRTL* response)
	: RequestChangeMode(mavlinkAbstraction, c2Handler, logger, response)
{
}

RequestRTL::~RequestRTL()
{
}

const char* RequestRTL::GetName()
{
	return "RTL";
}

float RequestRTL::getMode()
{
	return COPTER_MODE_RTL;
}