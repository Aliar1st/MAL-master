#include <gtest/gtest.h>

#include "Utilities/MathUtil.h"

namespace Utilities_Tests
{
	class MathUtil_Test : public ::testing::Test
	{
	public:

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(MathUtil_Test, NumberOfDigits)
	{
		ASSERT_EQ(MathUtil::NumberOfDigits(0), 1);

		ASSERT_EQ(MathUtil::NumberOfDigits(7), 1);
		ASSERT_EQ(MathUtil::NumberOfDigits(-7), 1);
		ASSERT_EQ(MathUtil::NumberOfDigits(-708), 3);
	}

	TEST_F(MathUtil_Test, DistanceBetweenGeographicPoints)
	{
		const int32_t latitude1 = 503761210;
		const int32_t longitude1 = 118815390;

		const int32_t latitude2 = 504660530;
		const int32_t longitude2 = 118815390;

		float distance = MathUtil::DistanceBetweenGeographicPoints(latitude1, longitude1, latitude2, longitude2);

		ASSERT_FLOAT_EQ(distance, 9999.9834f);
	}

	TEST_F(MathUtil_Test, DistanceBetweenAltitudes)
	{
		const int32_t altitude1 = 1000;
		const int32_t altitude2 = 2897;

		float distance = MathUtil::DistanceBetweenAltitudes(altitude1, altitude2);

		ASSERT_FLOAT_EQ(distance, 1.897f);
	}

	TEST_F(MathUtil_Test, DistanceBetweenAngles)
	{
		uint16_t angle1 = 4500;
		uint16_t angle2 = 18000;

		float distance = MathUtil::DistanceBetweenAngles(angle1, angle2);

		ASSERT_FLOAT_EQ(distance, 135.0f);

		angle1 = 4500;
		angle2 = 31500;

		distance = MathUtil::DistanceBetweenAngles(angle1, angle2);

		ASSERT_FLOAT_EQ(distance, 90.0f);
	}
}