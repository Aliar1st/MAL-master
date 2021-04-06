#pragma once

#include "DeviceData/DeviceDataGPSGlobalOrigin.h"
#include "ResponseGetDeviceDataBase.h"

/**
 * @brief	A response of the "GetDeviceData" request to retrieve device data about the GPS global origin
 */
struct ResponseGetDeviceDataGPSGlobalOrigin : public ResponseGetDeviceDataBase
{
protected:

	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) override;

public:

	DeviceDataGPSGlobalOrigin deviceData;

	/**
	 * @brief	The destructor of ResponseGetDeviceDataGPSGlobalOrigin
	 */
	virtual ~ResponseGetDeviceDataGPSGlobalOrigin() override;
};