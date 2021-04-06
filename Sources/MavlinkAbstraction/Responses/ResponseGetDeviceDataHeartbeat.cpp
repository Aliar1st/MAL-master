#include "ResponseGetDeviceDataHeartbeat.h"

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataHeartbeat.h"

ResponseGetDeviceDataHeartbeat::~ResponseGetDeviceDataHeartbeat()
{
}

RequestGetDeviceDataBase* ResponseGetDeviceDataHeartbeat::getRequest(Logger* logger)
{
	return new RequestGetDeviceDataHeartbeat(logger, this);
}