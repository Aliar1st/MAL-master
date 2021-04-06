#include "StringUtil.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "ConversionUtil.h"

int StringUtil::Strcmp(const char* str1, const char* str2, bool caseSensitive)
{
	if (caseSensitive)
	{
		return strcmp(str1, str2);
	}

#ifdef _WIN32
	return _stricmp(str1, str2);
#elif __linux__
	return strcasecmp(str1, str2);
#endif
}

size_t StringUtil::Strlen(const char* str)
{
	return strlen(str);
}

char* StringUtil::Strcat(const char* str1, const char* str2)
{
	// Calculate the length of both strings
	size_t str1Len = Strlen(str1);
	size_t str2Len = Strlen(str2);

	// Allocate memory
	char* result = new char[str1Len + str2Len + 1];

	// Assign all character of the both strings
	memcpy(result, str1, sizeof(char) * str1Len);
	memcpy(result + str1Len, str2, sizeof(char) * (str2Len + 1));

	// Return the result
	return result;
}

char* StringUtil::Strcpy(const char* str)
{
	if (!str)
	{
		return NULL;
	}

	return Strcpy(str, strlen(str));
}

char* StringUtil::Strcpy(const char* str, size_t length)
{
	if (!str)
	{
		return NULL;
	}

	// Allocate memory for the new string
	char* result = new char[length + 1];

	// Copy the content and append termination symbol
	memcpy(result, str, sizeof(char) * length);
	result[length] = '\0';

	return result;
}

size_t StringUtil::Strpos(const char* string, const char* substring, bool* found)
{
	size_t stringLength = Strlen(string);
	size_t substringLength = Strlen(substring);

	if (substringLength <= stringLength)
	{
		for (size_t stringIndex = 0; stringIndex <= stringLength - substringLength; stringIndex++)
		{
			size_t substringIndex = 0;

			// Compare both strings
			while (substringIndex < substringLength && string[stringIndex + substringIndex] == substring[substringIndex])
			{
				substringIndex++;
			}

			// Check if the substring was found as a whole
			if (substringIndex == substringLength)
			{
				*found = true;

				return stringIndex;
			}
		}
	}

	// Substring not found
	*found = false;

	return 0;
}

size_t StringUtil::Levenshtein(const char* str1, const char* str2, bool caseSensitive)
{
	size_t str1Length = Strlen(str1);
	size_t str2Length = Strlen(str2);

	if (str1Length > str2Length)
	{
		return Levenshtein(str2, str1, caseSensitive);
	}

	size_t* levDistArray = new size_t[str1Length + 1];

	for (size_t str1Index = 0; str1Index <= str1Length; str1Index++)
	{
		levDistArray[str1Index] = str1Index;
	}

	for (size_t str2Index = 0; str2Index < str2Length; str2Index++)
	{
		size_t prevDiagonal = levDistArray[0];

		levDistArray[0]++;

		for (size_t str1Index = 0; str1Index < str1Length; str1Index++)
		{
			size_t prevDiagonalSave = levDistArray[str1Index + 1];
			char str1Char = str1[str1Index];
			char str2Char = str2[str2Index];

			if (!caseSensitive)
			{
				str1Char = static_cast<char>(tolower(str1Char));
				str2Char = static_cast<char>(tolower(str2Char));
			}

			if (str1Char == str2Char)
			{
				levDistArray[str1Index + 1] = prevDiagonal;
			}
			else
			{
				levDistArray[str1Index + 1] = std::min(std::min(levDistArray[str1Index], levDistArray[str1Index + 1]), prevDiagonal) + 1;
			}

			prevDiagonal = prevDiagonalSave;
		}
	}

	size_t levDist = levDistArray[str1Length];

	delete[] levDistArray;

	return levDist;
}

const char* StringUtil::GetTimeStamp(const char* timeStampFormat)
{
	// Get local time
	std::tm localTime{};
	std::time_t time = std::time(0);

#ifdef _WIN32
	localtime_s(&localTime, &time);
#elif __linux__
	localtime_r(&time, &localTime);
#endif

	// Create time stamp string stream
	std::stringstream timeStampStringStream;

	timeStampStringStream << std::put_time(&localTime, timeStampFormat);

	return Strcpy(timeStampStringStream.str().c_str());
}

bool StringUtil::IsInt(const char* str)
{
	size_t strLength = Strlen(str);

	// Check for an empty string
	if (strLength == 0)
	{
		return false;
	}

	for (size_t i = 0; i < strLength; i++)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			// Check if the first sign is "-" or "+"
			if (i != 0 || (str[i] != '-' && str[i] != '+'))
			{
				return false;
			}
		}
	}

	return true;
}

bool StringUtil::IsUnsignedInt(const char* str)
{
	// Check if int
	if (!IsInt(str))
	{
		return false;
	}

	// Check if unsigned
	int convertedStr = ConversionUtil::StrToInt(str);

	return convertedStr >= 0;
}

bool StringUtil::IsFloat(const char* str)
{
	size_t strLength = strlen(str);
	bool decimalSeparatorFound = false;

	// Check for an empty string
	if (strLength == 0)
	{
		return false;
	}

	for (size_t i = 0; i < strLength; i++)
	{
		// Check for a single existence of the decimal separator
		if (str[i] == '.')
		{
			if (decimalSeparatorFound)
			{
				return false;
			}

			decimalSeparatorFound = true;
		}
		else if (str[i] < '0' || str[i] > '9')
		{
			// Check if the first sign is "-" or "+"
			if (i != 0 || (str[i] != '-' && str[i] != '+'))
			{
				return false;
			}
		}
	}

	return true;
}