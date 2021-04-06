#include "RequestLand.h"

RequestLand::RequestLand(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseLand* response)
	: RequestChangeMode(mavlinkAbstraction, c2Handler, logger, response)
{
}

RequestLand::~RequestLand()
{
}

const char* RequestLand::GetName()
{
	return "Land";
}

float RequestLand::getMode()
{
	return COPTER_MODE_LAND;
}