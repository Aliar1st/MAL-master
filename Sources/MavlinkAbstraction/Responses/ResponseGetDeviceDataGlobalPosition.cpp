#include "ResponseGetDeviceDataGlobalPosition.h"

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataGlobalPosition.h"

ResponseGetDeviceDataGlobalPosition::~ResponseGetDeviceDataGlobalPosition()
{
}

RequestGetDeviceDataBase* ResponseGetDeviceDataGlobalPosition::getRequest(Logger* logger)
{
	return new RequestGetDeviceDataGlobalPosition(logger, this);
}