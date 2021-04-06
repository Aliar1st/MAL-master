#include "ConversionUtil.h"

#include <cwchar>
#include <sstream>
#include <string>
#include "MathUtil.h"
#include "StringUtil.h"

#ifdef _WIN32

#include <Windows.h>

#endif

int ConversionUtil::StrToInt(const char* str)
{
	bool negative = false;
	size_t index = 0;

	// Check if negative
	if (str[0] == '-' || str[0] == '+')
	{
		if (str[0] == '-')
		{
			negative = true;
		}

		index++;
	}

	return (negative ? -1 : 1) * static_cast<int>(StrToUnsignedInt(str + index));
}

char* ConversionUtil::IntToStr(const int integer)
{
	int integerCopy = integer;

	// Account for the polarity of the integer
	bool negative = integerCopy < 0;
	size_t polaritySignLength = negative ? 1 : 0;

	// Create the resulting array
	size_t stringLength = polaritySignLength + MathUtil::NumberOfDigits(integerCopy) + 1;
	char* result = new char[stringLength];

	// If the integer's polarity is negative, set the first array character as being the negative sign
	if (negative)
	{
		result[0] = '-';
		integerCopy = -integerCopy;
	}

	// Fill the array
	for (size_t i = stringLength - 2; i >= polaritySignLength; i--)
	{
		result[i] = static_cast<char>('0' + integerCopy % 10);
		integerCopy /= 10;

		// Prevent overflow
		if (!i)
		{
			break;
		}
	}

	// Add the terminating character
	result[stringLength - 1] = '\0';

	// Return the converted char array
	return result;
}

int ConversionUtil::StrToUnsignedInt(const char* str)
{
	unsigned int result = 0;
	size_t index = 0;

	// Account for a possible "+" sign
	if (str[0] == '+')
	{
		index++;
	}

	// Handle all numerics
	while (str[index])
	{
		result = result * 10 + str[index] - '0';
		index++;
	}

	return result;
}

char* ConversionUtil::UnsignedIntToStr(const unsigned int integer)
{
	unsigned int integerCopy = integer;

	// Create the resulting array
	size_t stringLength = MathUtil::NumberOfDigits(integerCopy) + 1;
	char* result = new char[stringLength];

	// Fill the array
	size_t i = stringLength - 2;

	while (true)
	{
		result[i] = static_cast<char>('0' + integerCopy % 10);
		integerCopy /= 10;

		// Prevent overflow
		if (!i)
		{
			break;
		}

		i--;
	}

	// Add the terminating character
	result[stringLength - 1] = '\0';

	// Return the converted char array
	return result;
}

float ConversionUtil::StrToFloat(const char* str)
{
	return std::stof(str);
}

wchar_t* ConversionUtil::StrToWStr(const char* str)
{
	// Get the length of the resulting wchar array
	size_t lengthOfResult = StringUtil::Strlen(str) + 1;

	// Create the array and copy data into it
	wchar_t* result = new wchar_t[lengthOfResult];

#ifdef _WIN32
	MultiByteToWideChar(0, 0, str, -1, result, static_cast<int>(lengthOfResult));
#elif __linux__
	mbstowcs(result, str, lengthOfResult);
#endif // __linux__

	return result;
}

char* ConversionUtil::WStrToStr(const wchar_t* wCharStr)
{
	// Get the length of the resulting array
	size_t lengthOfResult = wcslen(wCharStr) + 1;

	// Create the array and copy data into it
	char* result = new char[lengthOfResult];

#ifdef _WIN32
	WideCharToMultiByte(0, 0, wCharStr, -1, result, static_cast<int>(lengthOfResult), NULL, NULL);
#elif __linux__
	wcstombs(result, wCharStr, lengthOfResult);
#endif // __linux__

	return result;
}