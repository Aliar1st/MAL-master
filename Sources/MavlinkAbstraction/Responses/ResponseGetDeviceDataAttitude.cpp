#include "ResponseGetDeviceDataAttitude.h"

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataAttitude.h"

ResponseGetDeviceDataAttitude::~ResponseGetDeviceDataAttitude()
{
}

RequestGetDeviceDataBase* ResponseGetDeviceDataAttitude::getRequest(Logger* logger)
{
	return new RequestGetDeviceDataAttitude(logger, this);
}