#pragma once

/**
 * @brief	A fact of the EXS
 */
class EXSFact
{
private:

	char* type;
	char* value;
	char* context;

	/**
	 * @brief	Extracts a single data from a fact string
	 *
	 * @param[in]	factString	The fact string to extract the data from
	 * @param[in]	dataField	The name of the data field
	 *
	 * @return A dynamically allocated string containing the value of the data; NULL if there was no data of the given name or if there was a formatting mismatch
	 */
	char* extractDataFromString(const char* factString, const char* dataField);

public:

	/**
	 * @brief	The constructor of EXSFact.
	 *
	 * @param[in]	factString	The fact string to extract the data from
	 */
	EXSFact(const char* factString);

	/**
	 * @brief	The constructor of EXSFact.
	 *
	 * @param[in]	type		The type of the fact
	 * @param[in]	context		The context of the fact
	 * @param[in]	value		The value of the fact
	 */
	EXSFact(const char* type, const char* context, const char* value);

	/**
	 * @brief	The copy constructor of EXSFact.
	 *
	 * @param[in]	other	The fact to copy the data from
	 */
	EXSFact(const EXSFact& other);

	/**
	 * @brief	The destructor of EXSFact.
	 */
	~EXSFact();

	char* GetType();

	char* GetContext();

	char* GetValue();

	/**
	 * @brief	Sets the value of this fact
	 *
	 * @param[in]	value	The value to be set
	 */
	void SetValue(const char* value);

	/**
	 * @brief	Returns this fact's fact string
	 *
	 * @return	The fact string. This string is dynamically allocated and should be freed using delete[].
	 *			NULL if the fact does not contain valid data.
	 */
	char* GetFactString();

	/**
	 * @brief	Returns whether the fact contains valid data
	 *
	 * @return	True if the fact contains valid data; False otherwise
	 */
	bool ContainsValidData();
};