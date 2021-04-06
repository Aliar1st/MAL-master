#include "MathUtil.h"

#include <cmath>

size_t MathUtil::NumberOfDigits(const int integer)
{
	size_t result = 1;

	for (int integerCopy = abs(integer); integerCopy >= 10; result++)
	{
		integerCopy /= 10;
	}

	return result;
}

float MathUtil::DistanceBetweenGeographicPoints(int32_t latitude1, int32_t longitude1, int32_t latitude2, int32_t longitude2)
{
	double latitude1Rad = ToRadians(latitude1 / 1.0e7);
	double latitude2Rad = ToRadians(latitude2 / 1.0e7);

	int64_t latitudeDifference = static_cast<int64_t>(latitude2) - static_cast<int64_t>(latitude1);
	int64_t longitudeDifference = static_cast<int64_t>(longitude2) - static_cast<int64_t>(longitude1);

	double deltaLatitudeRad = ToRadians(static_cast<double>(latitudeDifference) / 1.0e7);
	double deltaLongitudeRad = ToRadians(static_cast<double>(longitudeDifference) / 1.0e7);

	double a = pow(sin(deltaLatitudeRad / 2.0), 2.0) + pow(sin(deltaLongitudeRad / 2.0), 2.0) * cos(latitude1Rad) * cos(latitude2Rad);
	double c = 2.0 * atan2(sqrt(a), sqrt(1 - a));

	return static_cast<float>(EARTH_RADIUS * c);
}

float MathUtil::DistanceBetweenAltitudes(int32_t altitude1, int32_t altitude2)
{
	float absDistance = static_cast<float>(fabs(altitude2 - altitude1));

	return absDistance / 1000.0f;
}

float MathUtil::DistanceBetweenAngles(uint16_t angle1, uint16_t angle2)
{
	int absDistance = abs(static_cast<int32_t>(angle2) - static_cast<int32_t>(angle1));

	float shortestDistance = static_cast<float>(Modulo(absDistance + 18000, 36000) - 18000);

	return static_cast<float>(fabs(shortestDistance / 100.0f));
}