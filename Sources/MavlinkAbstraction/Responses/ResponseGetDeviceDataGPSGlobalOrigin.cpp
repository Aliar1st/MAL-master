#include "ResponseGetDeviceDataGPSGlobalOrigin.h"

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataGPSGlobalOrigin.h"

ResponseGetDeviceDataGPSGlobalOrigin::~ResponseGetDeviceDataGPSGlobalOrigin()
{
}

RequestGetDeviceDataBase* ResponseGetDeviceDataGPSGlobalOrigin::getRequest(Logger* logger)
{
	return new RequestGetDeviceDataGPSGlobalOrigin(logger, this);
}