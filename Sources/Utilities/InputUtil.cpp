#include "InputUtil.h"

#include <cstdio>
#include "ConversionUtil.h"
#include "StringUtil.h"

char* InputUtil::ReadLineFromConsole()
{
	char* result = nullptr;
	size_t resultLength = 0;

	do
	{
		// Copy the already read chars into a new array
		char* longerResult = new char[resultLength + 1];

		if (result)
		{
			for (unsigned long i = 0; i < resultLength; i++)
			{
				longerResult[i] = result[i];
			}
		}

		delete[] result;
		result = longerResult;
		resultLength++;

		result[resultLength - 1] = static_cast<char>(getchar());
	}
	while (result[resultLength - 1] != '\0' && result[resultLength - 1] != '\n');

	// Set the termination character
	result[resultLength - 1] = '\0';

	return result;
}

int InputUtil::ReadIntFromConsole()
{
	char* readLine = ReadLineFromConsole();
	int result = 0;

	if (StringUtil::IsInt(readLine))
	{
		result = ConversionUtil::StrToInt(readLine);
	}

	delete[] readLine;

	return result;
}

float InputUtil::ReadFloatFromConsole()
{
	char* readLine = ReadLineFromConsole();
	float result = 0.0f;

	if (StringUtil::IsFloat(readLine))
	{
		result = ConversionUtil::StrToFloat(readLine);
	}

	delete[] readLine;

	return result;
}