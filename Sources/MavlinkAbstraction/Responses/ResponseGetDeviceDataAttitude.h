#pragma once

#include "DeviceData/DeviceDataAttitude.h"
#include "ResponseGetDeviceDataBase.h"

/**
 * @brief	A response of the "GetDeviceData" request to retrieve device data about the attitude
 */
struct ResponseGetDeviceDataAttitude : public ResponseGetDeviceDataBase
{
protected:

	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) override;

public:

	DeviceDataAttitude deviceData;

	/**
	 * @brief	The destructor of ResponseGetDeviceDataAttitude
	 */
	virtual ~ResponseGetDeviceDataAttitude() override;
};