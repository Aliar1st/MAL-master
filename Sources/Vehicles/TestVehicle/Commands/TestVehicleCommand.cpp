#include "TestVehicleCommand.h"

const std::vector<const char*> TestVehicleCommand::GetAlternativeNames()
{
	return {};
}

const std::vector<const char*> TestVehicleCommand::GetNames()
{
	const char* name = GetName();
	std::vector<const char*> alternativeNames = GetAlternativeNames();

	alternativeNames.insert(alternativeNames.begin(), name);

	return alternativeNames;
}