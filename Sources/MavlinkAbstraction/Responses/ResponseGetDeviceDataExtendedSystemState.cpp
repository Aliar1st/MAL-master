#include "ResponseGetDeviceDataExtendedSystemState.h"

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataExtendedSystemState.h"

ResponseGetDeviceDataExtendedSystemState::~ResponseGetDeviceDataExtendedSystemState()
{
}

RequestGetDeviceDataBase* ResponseGetDeviceDataExtendedSystemState::getRequest(Logger* logger)
{
	return new RequestGetDeviceDataExtendedSystemState(logger, this);
}