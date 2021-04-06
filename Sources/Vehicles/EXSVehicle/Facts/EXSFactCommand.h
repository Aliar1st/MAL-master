#pragma once

#include "EXSFact.h"

class VehicleCommand;

/**
 * @brief	Fact of the EXS representing "Command"
 */
class EXSFactCommand : public EXSFact
{
public:

	/**
	 * @brief	The constructor of EXSFactCommand.
	 *
	 * @param	command		The command
	 */
	EXSFactCommand(VehicleCommand* command);
};