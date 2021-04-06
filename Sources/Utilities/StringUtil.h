#pragma once

#include <stddef.h>

/**
 * @brief	Contains functions that work on strings
 */
namespace StringUtil
{
	/**
	 * @brief	Compares two strings.
	 *
	 * @param[in]	str1			The first string.
	 * @param[in]	str2			The second string.
	 * @param		caseSensitive	True if lower and upper case are to be differentiated; False otherwise
	 *
	 * @return	0	Both strings are equal
	 *			<0	The first character that does not match has a lower value in str1 than in str2
	 *			>0	The first character that does not match has a higher value in str1 than in str2
	 */
	int Strcmp(const char* str1, const char* str2, bool caseSensitive = true);

	/**
	 * @brief	Calculates the length of the given string excluding the termination character.
	 *
	 * @param[in]	str	The string.
	 *
	 * @return	The length of the string excluding the termination sign.
	 */
	size_t Strlen(const char* str);

	/**
	 * @brief	Appends two strings.
	 *
	 * @param[in]	str1	The first string.
	 * @param[in]	str2	The second string.
	 *
	 * @return	A string containing both given strings in succession.
	 */
	char* Strcat(const char* str1, const char* str2);

	/**
	 * @brief	Copies the content of a string.
	 *
	 * @param[in]	str	The string to copy the content from. May be NULL
	 *
	 * @return	A copy of the string. The string is dynamically allocated and must be freed using delete[]. NULL if str is NULL.
	 */
	char* Strcpy(const char* str);

	/**
	 * @brief	Creates a copy substring of a string with the given length.
	 *
	 * @param[in]	str		The string to copy the substring from. May be NULL
	 * @param		length	The length of the substring. If str is NULL, this value is ignored
	 *
	 * @return	A copy of the substring. The string is dynamically allocated and must be freed using delete[]. NULL if str is NULL.
	 */
	char* Strcpy(const char* str, size_t length);

	/**
	 * @brief	Find the position of the first occurrence of a substring in a string.
	 *
	 * @param[in]	string		The string to search in
	 * @param[in]	substring	The substring to search for.
	 * @param[out]	found		True if the substring was found; False otherwise
	 *
	 * @return	The starting position of the substring
	 */
	size_t Strpos(const char* string, const char* substring, bool* found);

	/**
	 * @brief	Calculates the Levenshtein distance between the given strings
	 *
	 * @param[in]	str1			First string
	 * @param[in]	str2			Second string
	 * @param		caseSensitive	True if lower and upper case are to be differentiated; False otherwise
	 *
	 * @return	Levenshtein distance between both strings.
	 */
	size_t Levenshtein(const char* str1, const char* str2, bool caseSensitive = true);

	/**
	 * @brief	Gets the current time stamp.
	 *
	 * @param[in]	timeStampFormat		Format of the time stamp. See https://en.cppreference.com/w/cpp/io/manip/put_time
	 *
	 * @return	The time stamp. The string is dynamically allocated and must be freed using delete[].
	 */
	const char* GetTimeStamp(const char* timeStampFormat);

	/**
	 * @brief	Checks if the given string is an integer.
	 *
	 * @param[in]	str	The string to check
	 *
	 * @return	True if the string is an integer; False otherwise.
	 */
	bool IsInt(const char* str);

	/**
	 * @brief	Checks if the given string is an unsigned integer.
	 *
	 * @param[in]	str	The string to check
	 *
	 * @return	True if the string is an unsigned integer; False otherwise.
	 */
	bool IsUnsignedInt(const char* str);

	/**
	 * @brief	Checks if the given string is a floating point number.
	 *
	 * @param[in]	str	The string to check
	 *
	 * @return	True if the string is a floating point number; False otherwise.
	 */
	bool IsFloat(const char* str);
}