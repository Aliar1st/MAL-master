#pragma once

#include <cstring>
#include <type_traits>

/**
 * @brief	Contains functions to convert between data types
 */
namespace ConversionUtil
{
	/**
	 * @brief	Converts a string to an integer.
	 *
	 * @param[in]	str	The string to convert. This string must represent a valid integer.
	 *
	 * @return	The converted integer.
	 */
	int StrToInt(const char* str);

	/**
	 * @brief	Converts an integer to a string.
	 *
	 * @param	integer	The integer to convert.
	 *
	 * @return	The converted string. The string is dynamically allocated and must be freed using delete[].
	 */
	char* IntToStr(const int integer);

	/**
	 * @brief	Converts a string to an unsigned integer.
	 *
	 * @param[in]	str	The string to convert. This string must represent a valid unsigned integer.
	 *
	 * @return	The converted unsigned integer.
	 */
	int StrToUnsignedInt(const char* str);

	/**
	 * @brief	Converts an unsigned integer to a string.
	 *
	 * @param	integer	The unsigned integer to convert.
	 *
	 * @return	The converted string. The string is dynamically allocated and must be freed using delete[].
	 */
	char* UnsignedIntToStr(const unsigned int integer);

	/**
	 * @brief	Converts a string to a float.
	 *
	 * @param[in]	str	The string to convert. This string must represent a valid float.
	 *
	 * @return	The converted float.
	 */
	float StrToFloat(const char* str);

	/**
	 * @brief	Converts a string to a wide character string.
	 *
	 * @param[in]	str	The string to convert
	 *
	 * @return	The converted wide character string. The wide character string is dynamically allocated and must be freed using delete[].
	 */
	wchar_t* StrToWStr(const char* str);

	/**
	 * @brief	Converts a wide character string to a string.
	 *
	 * @param[in]	wCharStr	The wide character string to convert
	 *
	 * @return	The converted string. The wide character string is dynamically allocated and must be freed using delete[].
	 */
	char* WStrToStr(const wchar_t* wCharStr);

	/**
	 * @brief	Copies the bytes contained in one variable into another variable.
	 *
	 * @param[in]	input	The variable to copy the bits from.
	 * @param[out]	output	A pointer to the variable to write the bits into.
	 *
	 * @tparam	The type of the output pointer. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	void Convert(void* input, T* output)
	{
		memcpy((void*) output, input, sizeof(T));
	}

	/**
	 * @brief	Copies the bytes contained in one variable into another variable.
	 *
	 * @param		input	The variable to copy the bits from.
	 * @param[out]	output	A pointer to the variable to write the bits into.
	 *
	 * @tparam	The type of the input. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	void Convert(T input, void* output)
	{
		memcpy(output, &input, sizeof(T));
	}
}