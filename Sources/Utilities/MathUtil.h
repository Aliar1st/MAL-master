#pragma once

#include <stddef.h>
#include <stdint.h>
#include <type_traits>

/**
 * @brief	Contains mathematical functions
 */
namespace MathUtil
{
	// Pi
	constexpr static const double PI = 3.14159265358979323846;

	// Earth radius in m
	constexpr static const double EARTH_RADIUS = 6'371'000.785;

	/**
	 * @brief	Returns the number of digits of an integer. Note that the sign is not counted.
	 *
	 * @param	integer	The integer.
	 *
	 * @return	The total number of digits of the given integer.
	 */
	size_t NumberOfDigits(const int integer);

	/**
	 * @brief	Converts a radians value to degree
	 *
	 * @param	radians		Value to convert
	 *
	 * @return	Converted value
	 *
	 * @tparam	The type of the value
	 */
	template<typename T>
	T ToDegree(T radians)
	{
		return static_cast<T>(radians * 180.0 / PI);
	}

	/**
	 * @brief	Converts a degree value to radians
	 *
	 * @param	degree	Value to convert
	 *
	 * @return	Converted value
	 *
	 * @tparam	The type of the value
	 */
	template<typename T>
	T ToRadians(T degree)
	{
		return static_cast<T>(degree * PI / 180.0);
	}

	/**
	 * @brief	Returns the remainder of dividend / divisor (i.e. dividend mod divisor)
	 *
	 * @param	dividend	Dividend
	 * @param	divisor		Divisor
	 *
	 * @return	dividend mod divisor
	 *
	 * @tparam	The type of dividend and divisor.
	 */
	template<typename T>
	T Modulo(T dividend, T divisor)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return static_cast<T>(dividend - divisor * floor(dividend / divisor));
		}
		else
		{
			return (dividend % divisor + divisor) % divisor;
		}
	}

	/**
	 * @brief	Calculates the distance between two geographic points
	 *
	 * @param	latitude1		Latitude of the first point [degE7]
	 * @param	longitude1		Longitude of the first point [degE7]
	 * @param	latitude2		Latitude of the second point [degE7]
	 * @param	longitude2		Longitude of the second point [degE7]
	 *
	 * @return	Distance [m]
	 */
	float DistanceBetweenGeographicPoints(int32_t latitude1, int32_t longitude1, int32_t latitude2, int32_t longitude2);

	/**
	 * @brief	Calculates the distance between two altitudes
	 *
	 * @param	altitude1		First altitude [mm]
	 * @param	altitude2		Second altitude [mm]
	 *
	 * @return	Distance [m]
	 */
	float DistanceBetweenAltitudes(int32_t altitude1, int32_t altitude2);

	/**
	 * @brief	Calculates the distance between two angles
	 *
	 * @param	angle1	First angle [cdeg]
	 * @param	angle2	Second angle [cdeg]
	 *
	 * @return	Distance [deg]
	 */
	float DistanceBetweenAngles(uint16_t angle1, uint16_t angle2);
}