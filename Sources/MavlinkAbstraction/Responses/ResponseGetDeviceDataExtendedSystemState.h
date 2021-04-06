#pragma once

#include "DeviceData/DeviceDataExtendedSystemState.h"
#include "ResponseGetDeviceDataBase.h"

/**
 * @brief	A response of the "GetDeviceData" request to retrieve device data about the extended system state
 */
struct ResponseGetDeviceDataExtendedSystemState : public ResponseGetDeviceDataBase
{
protected:

	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) override;

public:

	DeviceDataExtendedSystemState deviceData;

	/**
	 * @brief	The destructor of ResponseGetDeviceDataExtendedSystemState
	 */
	virtual ~ResponseGetDeviceDataExtendedSystemState() override;
};