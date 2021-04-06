#include "ResponseGetDeviceDataLocalPositionNED.h"

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataLocalPositionNED.h"

ResponseGetDeviceDataLocalPositionNED::~ResponseGetDeviceDataLocalPositionNED()
{
}

RequestGetDeviceDataBase* ResponseGetDeviceDataLocalPositionNED::getRequest(Logger* logger)
{
	return new RequestGetDeviceDataLocalPositionNED(logger, this);
}