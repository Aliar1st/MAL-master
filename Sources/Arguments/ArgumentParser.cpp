#include "ArgumentParser.h"

#include "Utilities/StringUtil.h"

bool ArgumentParser::Parse(size_t* argc, char** argv[], Arguments* arguments)
{
	preParse();

	// Set all arguments to parsed
	bool* argParsed = new bool[*argc];

	for (size_t i = 0; i < *argc; i++)
	{
		argParsed[i] = true;
	}

	// Parse
	for (size_t i = 0; i < *argc;)
	{
		size_t argumentsParsed;

		if (!parseArgument(*argc - i, &((*argv)[i]), &argumentsParsed, arguments))
		{
			// Arguments are missing
			delete[] argParsed;

			return false;
		}

		if (!argumentsParsed)
		{
			// Current argument was not parsed
			argParsed[i] = false;
			i++;
		}
		else
		{
			// At least one argument was parsed. Continue with the next unparsed argument
			i += argumentsParsed;
		}
	}

	// Check if all arguments were provided
	if (postParse(arguments))
	{
		removeArguments(argc, argv, argParsed);
		delete[] argParsed;

		return true;
	}

	delete[] argParsed;

	return false;
}

bool ArgumentParser::isArgument(const char* arg, std::vector<const char*> argumentStrings)
{
	for (size_t i = 0; i < argumentStrings.size(); i++)
	{
		if (!StringUtil::Strcmp(arg, argumentStrings[i]))
		{
			return true;
		}
	}

	return false;
}

bool ArgumentParser::isArgument(const char* arg, const char* argumentString)
{
	return isArgument(arg, std::vector<const char*> {argumentString});
}

void ArgumentParser::removeArguments(size_t* argc, char** argv[], bool* argParsed)
{
	// Count how many arguments were parsed
	size_t argParsedCount = 0;

	for (size_t i = 0; i < *argc; i++)
	{
		if (argParsed[i])
		{
			argParsedCount++;
		}
	}

	char** newArgv = new char*[*argc - argParsedCount];
	size_t newArgcIndex = 0;

	for (size_t i = 0; i < *argc; i++)
	{
		if (!argParsed[i])
		{
			newArgv[newArgcIndex] = StringUtil::Strcpy((*argv)[i]);
			newArgcIndex++;
		}

		delete[] (*argv)[i];
	}

	delete[] *argv;

	*argc = *argc - argParsedCount;
	*argv = newArgv;
}