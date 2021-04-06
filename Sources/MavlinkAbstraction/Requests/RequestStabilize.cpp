#include "RequestStabilize.h"

RequestStabilize::RequestStabilize(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseStabilize* response)
	: RequestChangeMode(mavlinkAbstraction, c2Handler, logger, response)
{
}

RequestStabilize::~RequestStabilize()
{
}

const char* RequestStabilize::GetName()
{
	return "Stabilize";
}

float RequestStabilize::getMode()
{
	return COPTER_MODE_STABILIZE;
}