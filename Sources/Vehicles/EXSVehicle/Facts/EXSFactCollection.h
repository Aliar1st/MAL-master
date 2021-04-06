#pragma once

#include <vector>
#include "EXSFact.h"

/**
 * @brief	A collection of facts of the EXS
 */
class EXSFactCollection
{
private:

	char* exsPath;
	char* filePath;

	// The vector of all facts. This vector does not contain duplicates
	// Additionally, this vector is sorted by the context and then by the type
	std::vector<EXSFact*> factVector;

	/**
	 * @brief	Searches for a fact and gets its position
	 *
	 * @param[in]	type	Type of the fact
	 * @param[in]	context	Context of the fact
	 * @param[out]	index	Index of the fact if it was found; Otherwise index where the fact should be
	 *
	 * @return	True if the fact was found; False otherwise
	 */
	bool getIndexOfFact(const char* type, const char* context, size_t* index);

	/**
	 * @brief	Compares two facts
	 *
	 * @param[in]	fact1		The first fact
	 * @param[in]	type2		The type of the second fact
	 * @param[in]	context		The context of the second fact
	 *
	 * @return	0	Both facts are equal
	 *			<0	The first fact is smaller than the second one
	 *			>0	The first fact is greater than the second one
	 */
	int compareFacts(EXSFact* fact1, const char* type2, const char* context2);

public:

	/**
	 * @brief	The constructor of EXSFactCollection.
	 *
	 * @param[in]	exsPath		Path of the EXS
	 */
	EXSFactCollection(const char* exsPath);

	/**
	 * @brief	The destructor of EXSFactCollection.
	 */
	~EXSFactCollection();

	/**
	 * @brief	Updates the facts by reading from the file. All facts that are set before reading will be reset.
	 */
	void Read();

	/**
	 * @brief	Writes all facts to the file.
	 */
	void Write();

	/**
	 * @brief	Removes all facts from the fact collection
	 */
	void RemoveFacts();

	/**
	 * @brief	Sets or inserts a fact
	 */
	void SetFact(EXSFact fact);

	/**
	 * @brief	Sets or inserts multiple facts
	 */
	void SetFacts(EXSFactCollection& factCollection);

	/**
	 * @brief	Returns the value of a fact of the given type and context
	 *
	 * @param[in]	type		The type of the fact to get the value from
	 * @param[in]	context		The context of the fact to get the value from
	 *
	 * @return	The value of the fact; NULL if no fact of the given type and context was found
	 */
	char* GetFactValue(const char* type, const char* context);
};