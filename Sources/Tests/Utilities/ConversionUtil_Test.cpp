#include <gtest/gtest.h>

#include <tuple>
#include "Utilities/ConversionUtil.h"

namespace Utilities_Tests
{
	class ConverionsUtil_Test : public ::testing::Test
	{
	protected:

		const std::vector<std::tuple<const int, const char*>> INT_STRING_PAIRS = {
			{3, "3"},
			{17738, "17738"},
			{0, "0"},
			{-7, "-7"},
			{-71084, "-71084"}
		};

		const std::vector<std::tuple<const unsigned int, const char*>> UNSIGNED_INT_STRING_PAIRS = {
			{3, "3"},
			{17738, "17738"},
			{0, "0"}
		};

		const std::vector<std::tuple<const float, const char*>> FLOAT_STRING_PAIRS = {
			{3.1f, "3.1"},
			{300000000000.1f, "300000000000.1"},
			{-3.1f, "-3.1"}
		};

		const std::vector<std::tuple<const wchar_t*, const char*>> WSTRING_STRING_PAIRS = {
			{L"", ""},
			{L"Test", "Test"},
			{L"Test with spaces", "Test with spaces"}
		};

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
	TEST_F(ConverionsUtil_Test, StrToInt)
	{
		for (size_t i = 0; i < INT_STRING_PAIRS.size(); i++)
		{
			const int intValue = std::get<0>(INT_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(INT_STRING_PAIRS[i]);

			ASSERT_EQ(ConversionUtil::StrToInt(stringValue), intValue) << "Index: " << i;
		}
	}

	TEST_F(ConverionsUtil_Test, IntToStr)
	{
		for (size_t i = 0; i < INT_STRING_PAIRS.size(); i++)
		{
			const int intValue = std::get<0>(INT_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(INT_STRING_PAIRS[i]);

			char* convertedString = ConversionUtil::IntToStr(intValue);

			ASSERT_STREQ(convertedString, stringValue) << "Index: " << i;
			delete[] convertedString;
		}
	}

	TEST_F(ConverionsUtil_Test, StrToUnsignedInt)
	{
		for (size_t i = 0; i < UNSIGNED_INT_STRING_PAIRS.size(); i++)
		{
			const int unsignedIntValue = std::get<0>(UNSIGNED_INT_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(UNSIGNED_INT_STRING_PAIRS[i]);

			ASSERT_EQ(ConversionUtil::StrToUnsignedInt(stringValue), unsignedIntValue) << "Index: " << i;
		}
	}

	TEST_F(ConverionsUtil_Test, UnsignedIntToStr)
	{
		for (size_t i = 0; i < UNSIGNED_INT_STRING_PAIRS.size(); i++)
		{
			const int unsignedIntValue = std::get<0>(UNSIGNED_INT_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(UNSIGNED_INT_STRING_PAIRS[i]);

			char* convertedString = ConversionUtil::UnsignedIntToStr(unsignedIntValue);

			ASSERT_STREQ(convertedString, stringValue) << "Index: " << i;
			delete[] convertedString;
		}
	}

	TEST_F(ConverionsUtil_Test, StrToFloat)
	{
		for (size_t i = 0; i < FLOAT_STRING_PAIRS.size(); i++)
		{
			const float floatValue = std::get<0>(FLOAT_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(FLOAT_STRING_PAIRS[i]);

			ASSERT_EQ(ConversionUtil::StrToFloat(stringValue), floatValue) << "Index: " << i;
		}
	}

	TEST_F(ConverionsUtil_Test, StrToWStr)
	{
		for (size_t i = 0; i < WSTRING_STRING_PAIRS.size(); i++)
		{
			const wchar_t* wstringValue = std::get<0>(WSTRING_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(WSTRING_STRING_PAIRS[i]);

			ASSERT_STREQ(ConversionUtil::StrToWStr(stringValue), wstringValue) << "Index: " << i;
		}
	}

	TEST_F(ConverionsUtil_Test, WStrToStr)
	{
		for (size_t i = 0; i < WSTRING_STRING_PAIRS.size(); i++)
		{
			const wchar_t* wstringValue = std::get<0>(WSTRING_STRING_PAIRS[i]);
			const char* stringValue = std::get<1>(WSTRING_STRING_PAIRS[i]);

			char* convertedString = ConversionUtil::WStrToStr(wstringValue);

			ASSERT_STREQ(convertedString, stringValue) << "Index: " << i;
			delete[] convertedString;
		}
	}
}