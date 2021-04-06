#pragma once

/**
 * @brief	Contains functions to receive input
 */
namespace InputUtil
{
	/**
	 * @brief	Reads a character sequence from the console until an enter is detected.
	 *
	 * @return	The read string. This string is dynamically allocated and must be freed via delete[].
	 */
	char* ReadLineFromConsole();

	/**
	 * @brief	Reads an integer from the console until an enter is detected.
	 *
	 * @return	The read integer. If the given console input does not represent an integer, 0 is returned.
	 */
	int ReadIntFromConsole();

	/**
	 * @brief	Reads a float from the console until an enter is detected.
	 *
	 * @return	The read float. If the given console input does not represent a float, 0.0f is returned.
	 */
	float ReadFloatFromConsole();
}