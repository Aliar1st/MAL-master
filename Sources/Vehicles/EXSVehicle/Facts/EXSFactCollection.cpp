#include "EXSFactCollection.h"

#include <fstream>
#include <sstream>
#include <string>
#include "Utilities/StringUtil.h"

EXSFactCollection::EXSFactCollection(const char* exsPath)
{
	// Set the paths
	this->exsPath = StringUtil::Strcpy(exsPath);
	filePath = StringUtil::Strcat(exsPath, "FACTS.clp");
}

EXSFactCollection::~EXSFactCollection()
{
	delete[] filePath;
	delete[] exsPath;

	RemoveFacts();
}

void EXSFactCollection::Read()
{
	// Clear all facts
	RemoveFacts();

	// Extract all facts from the file
	std::ifstream file(filePath);
	std::string line;

	while (std::getline(file, line))
	{
		EXSFact fact(line.c_str());

		SetFact(fact);
	}
}

void EXSFactCollection::Write()
{
	// Write to the file
	std::ofstream file(filePath);

	for (size_t i = 0; i < factVector.size(); i++)
	{
		char* factString = factVector[i]->GetFactString();

		file << factString << std::endl;
		delete[] factString;
	}
}

void EXSFactCollection::RemoveFacts()
{
	for (size_t i = 0; i < factVector.size(); i++)
	{
		delete factVector[i];
	}
	factVector.clear();
}

void EXSFactCollection::SetFact(EXSFact fact)
{
	// Check if valid fact
	if (!fact.ContainsValidData())
	{
		return;
	}

	size_t index;

	// Check if the fact already exists
	if (getIndexOfFact(fact.GetType(), fact.GetContext(), &index))
	{
		// Check if the value differs
		if (!StringUtil::Strcmp(fact.GetValue(), factVector[index]->GetValue()))
		{
			return;
		}

		// Update the fact's value
		factVector[index]->SetValue(fact.GetValue());
	}
	else
	{
		// Append in front of the found index
		factVector.insert(factVector.begin() + index, new EXSFact(fact));
	}
}

void EXSFactCollection::SetFacts(EXSFactCollection& factCollection)
{
	for (size_t i = 0; i < factCollection.factVector.size(); i++)
	{
		SetFact(*factCollection.factVector[i]);
	}
}

char* EXSFactCollection::GetFactValue(const char* type, const char* context)
{
	size_t index;

	if (!getIndexOfFact(type, context, &index))
	{
		return NULL;
	}

	return factVector[index]->GetValue();
}

bool EXSFactCollection::getIndexOfFact(const char* type, const char* context, size_t* index)
{
	if (factVector.empty())
	{
		*index = 0;

		return false;
	}

	/**
	 * Use binary search to find the fact
	 */
	size_t leftBound = 0;
	size_t rightBound = factVector.size() - 1;

	while (leftBound <= rightBound)
	{
		size_t indexToCheck = (leftBound + rightBound) / 2;
		EXSFact*& factToCheck = factVector[indexToCheck];

		// Compare the facts
		int compareValue = compareFacts(factToCheck, type, context);

		if (compareValue == 0)
		{
			*index = indexToCheck;

			return true;
		}
		else if (compareValue < 0)
		{
			// Continue searching on the right side
			leftBound = indexToCheck + 1;
		}
		else
		{
			// Check if 0 has been reached already to avoid an overflow
			if (!indexToCheck)
			{
				break;
			}

			// Continue searching on the left side
			rightBound = indexToCheck - 1;
		}
	}

	// Fact doesn't exist
	*index = leftBound;

	return false;
}

int EXSFactCollection::compareFacts(EXSFact* fact1, const char* type2, const char* context2)
{
	// Compare context
	int contextCmp = StringUtil::Strcmp(fact1->GetContext(), context2);

	if (contextCmp != 0)
	{
		return contextCmp;
	}

	// Compare type
	return StringUtil::Strcmp(fact1->GetType(), type2);
}