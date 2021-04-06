#pragma once

#include <cstring>
#include "MALArguments.h"

/**
 * @brief	An argument parser for all arguments
 */
class MALArgumentParser
{
public:

	/**
	 * @brief	Parses arguments and fills the provided argument object with the corresponding data
	 *
	 * @param		argc		The amount of arguments provided
	 * @param[in]	argv		All arguments
	 * @param[out]	arguments	After this function returns, contains all parsed arguments
	 *
	 * @return	True if the provided arguments were correctly parsed; False if any argument was missing.
	 */
	bool Parse(size_t argc, char* argv[], MALArguments* arguments);
};