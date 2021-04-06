#include "RequestGuided.h"

RequestGuided::RequestGuided(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseGuided* response)
	: RequestChangeMode(mavlinkAbstraction, c2Handler, logger, response)
{
}

RequestGuided::~RequestGuided()
{
}

const char* RequestGuided::GetName()
{
	return "Guided";
}

float RequestGuided::getMode()
{
	return COPTER_MODE_GUIDED;
}