#include <gtest/gtest.h>

#include "Utilities/StringUtil.h"

namespace Utilities_Tests
{
	class StringUtil_Test : public ::testing::Test
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
	TEST_F(StringUtil_Test, Strcmp)
	{
		ASSERT_EQ(StringUtil::Strcmp("Test", "Test"), 0);

		ASSERT_NE(StringUtil::Strcmp("Test", "Not Test"), 0);
		ASSERT_NE(StringUtil::Strcmp("Test", "TEST"), 0);
		ASSERT_EQ(StringUtil::Strcmp("Test", "TEST", false), 0);

		ASSERT_LT(StringUtil::Strcmp("e", "f"), 0);
		ASSERT_GT(StringUtil::Strcmp("f", "e"), 0);
	}

	TEST_F(StringUtil_Test, Strlen)
	{
		ASSERT_EQ(StringUtil::Strlen("a"), 1);
		ASSERT_EQ(StringUtil::Strlen("Test"), 4);
		ASSERT_EQ(StringUtil::Strlen(""), 0);
	}

	TEST_F(StringUtil_Test, Strcat)
	{
		const char* string1 = "Hello ";
		const char* string2 = "World";
		const char* stringConcatenated = "Hello World";

		char* concatenatedString = StringUtil::Strcat(string1, string2);

		ASSERT_STREQ(concatenatedString, stringConcatenated);
		delete[] concatenatedString;
	}

	TEST_F(StringUtil_Test, Strcpy)
	{
		const char* string = "Test";
		const char* shortenedString = "Tes";

		char* copiedString = StringUtil::Strcpy(string);

		ASSERT_STREQ(copiedString, string);
		delete[] copiedString;

		copiedString = StringUtil::Strcpy(string, 3);
		ASSERT_STREQ(copiedString, shortenedString);
		delete[] copiedString;
	}

	TEST_F(StringUtil_Test, Strpos)
	{
		bool found;
		size_t substringPosition;

		substringPosition = StringUtil::Strpos("Hello World", "Wor", &found);
		ASSERT_TRUE(found);
		ASSERT_EQ(substringPosition, 6);

		substringPosition = StringUtil::Strpos("Hello World", "Non-existent substring", &found);
		ASSERT_FALSE(found);

		substringPosition = StringUtil::Strpos("Hello World", "Hello World", &found);
		ASSERT_TRUE(found);
		ASSERT_EQ(substringPosition, 0);
	}

	TEST_F(StringUtil_Test, Levenshtein)
	{
		ASSERT_EQ(StringUtil::Levenshtein("", ""), 0);
		ASSERT_EQ(StringUtil::Levenshtein("a", ""), 1);
		ASSERT_EQ(StringUtil::Levenshtein("", "a"), 1);
		ASSERT_EQ(StringUtil::Levenshtein("Hello", "llo"), 2);
		ASSERT_EQ(StringUtil::Levenshtein("Hello", "Hello"), 0);
		ASSERT_EQ(StringUtil::Levenshtein("Hello", "HELLO"), 4);
		ASSERT_EQ(StringUtil::Levenshtein("Hello", "HELLO", false), 0);
	}

	TEST_F(StringUtil_Test, IsInt)
	{
		ASSERT_TRUE(StringUtil::IsInt("132453"));
		ASSERT_TRUE(StringUtil::IsInt("-132453"));
		ASSERT_TRUE(StringUtil::IsInt("-0"));

		ASSERT_FALSE(StringUtil::IsInt("3.1415"));
		ASSERT_FALSE(StringUtil::IsInt("3."));
		ASSERT_FALSE(StringUtil::IsInt("+-3"));
		ASSERT_FALSE(StringUtil::IsInt("This is not an integer"));
	}

	TEST_F(StringUtil_Test, IsUnsignedInt)
	{
		ASSERT_TRUE(StringUtil::IsUnsignedInt("132453"));
		ASSERT_TRUE(StringUtil::IsUnsignedInt("+132453"));
		ASSERT_TRUE(StringUtil::IsUnsignedInt("0"));

		ASSERT_FALSE(StringUtil::IsUnsignedInt("3.1415"));
		ASSERT_FALSE(StringUtil::IsUnsignedInt("3."));
		ASSERT_FALSE(StringUtil::IsUnsignedInt("-3"));
		ASSERT_FALSE(StringUtil::IsUnsignedInt("This is not an integer"));
	}

	TEST_F(StringUtil_Test, IsFloat)
	{
		ASSERT_TRUE(StringUtil::IsFloat("132453"));
		ASSERT_TRUE(StringUtil::IsFloat("3.1415"));
		ASSERT_TRUE(StringUtil::IsFloat("3."));
		ASSERT_TRUE(StringUtil::IsFloat(".0"));
		ASSERT_TRUE(StringUtil::IsFloat("-132453"));
		ASSERT_TRUE(StringUtil::IsFloat("-3.1415"));
		ASSERT_TRUE(StringUtil::IsFloat("-3."));
		ASSERT_TRUE(StringUtil::IsFloat("-.0"));

		ASSERT_FALSE(StringUtil::IsFloat("+-3"));
		ASSERT_FALSE(StringUtil::IsFloat("3.14.15"));
		ASSERT_FALSE(StringUtil::IsFloat("This is not an float"));
	}
}