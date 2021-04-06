#include "ArgumentParser_Test.h"

#include "Utilities/StringUtil.h"

void Arguments_Tests::ArgumentParser_Test::SetUp()
{
}

void Arguments_Tests::ArgumentParser_Test::TearDown()
{
	for (size_t i = 0; i < argc; i++)
	{
		delete[] argv[i];
	}

	delete[] argv;
}

void Arguments_Tests::ArgumentParser_Test::setupArguments(std::vector<const char*> argumentVector)
{
	for (size_t i = 0; i < argc; i++)
	{
		delete[] argv[i];
	}

	delete[] argv;

	argc = argumentVector.size();
	argv = new char*[argc];

	for (size_t i = 0; i < argc; i++)
	{
		argv[i] = StringUtil::Strcpy(argumentVector[i]);
	}
}