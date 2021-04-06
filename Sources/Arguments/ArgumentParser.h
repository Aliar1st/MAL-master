#pragma once

#include <vector>
#include "Arguments.h"

/**
 * @brief	An abstract argument parser
 */
class ArgumentParser
{
private:

	/**
	 * @brief	Removes the arguments that have been parsed
	 *
	 * @param[in,out]	argc		Initially contains the amount of arguments provided. After this function returns, this value is decreased by the amount of arguments parsed
	 * @param[in,out]	argv		Initially contains all arguments. This array has to be created using new[].
	 *								After this function returns, contains all arguments that were not parsed. This new array is created using new[].
	 * @param[in]		argParsed	Array containing whether an argument was parsed
	 */
	void removeArguments(size_t* argc, char** argv[], bool* argParsed);

protected:

	constexpr static const char INDENT[] = "  ";

	/**
	 * @brief	Function that is called at the beginning of the parsing process
	 */
	virtual void preParse() = 0;

	/**
	 * @brief	Function that is called at the end of the parsing process
	 *
	 * @param[in]	arguments	Contains all parsed arguments
	 *
	 * @return	True if the parsing was successful; False if any arguments were missing
	 */
	virtual bool postParse(Arguments* arguments) = 0;

	/**
	 * @brief	Parses the next argument. Additional arguments may be parsed. This is indicated via argumentsParsed.
	 *
	 * @param		argc				The amount of arguments provided
	 * @param		argv				The array containing all remaining arguments
	 * @param[out]	argumentsParsed		After this function returns, contains the amount of arguments that were parsed.
	 * @param[out]	arguments			After this function returns, contains the data of the parsed argument if there was any
	 *
	 * @return	True if the argument was parsed without any errors; False if any arguments were missing
	 */
	virtual bool parseArgument(size_t argc, char* argv[], size_t* argumentsParsed, Arguments* arguments) = 0;

	/**
	 * @brief	Checks if the given argument is equal to one of the argument strings
	 *
	 * @param[in]	arg					Given argument
	 * @param		argumentStrings		Strings to compare the argument against
	 *
	 * @return	True if the argument is equal to one of the strings; False otherwise
	 */
	bool isArgument(const char* arg, std::vector<const char*> argumentStrings);

	/**
	 * @brief	Checks if the given argument is equal to the argument string
	 *
	 * @param[in]	arg					Given argument
	 * @param		argumentStrings		String to compare the argument against
	 *
	 * @return	True if the argument is equal to the string; False otherwise
	 */
	bool isArgument(const char* arg, const char* argumentString);

public:

	/**
	 * @brief	Parses arguments and fills the provided argument object with the corresponding data
	 *
	 * @param[in,out]	argc		Initially contains the amount of arguments provided. After this function returns, this value is decreased by the amount of arguments parsed
	 * @param[in,out]	argv		Initially contains all arguments. This array has to be created using new[].
	 *								After this function returns, contains all arguments that were not parsed. This new array is created using new[].
	 * @param[out]		arguments	After this function returns, contains all parsed arguments
	 *
	 * @return	True if the provided arguments were correctly parsed; False if any argument was missing.
	 *			If false, argc and argv is not modified.
	 */
	bool Parse(size_t* argc, char** argv[], Arguments* arguments);

	/**
	 * @brief	Prints the argument help list.
	 */
	virtual void PrintHelp() = 0;
};