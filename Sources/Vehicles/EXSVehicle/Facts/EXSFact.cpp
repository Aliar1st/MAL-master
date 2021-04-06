#include "EXSFact.h"

#include <string>
#include "Utilities/StringUtil.h"

EXSFact::EXSFact(const char* factString)
{
	// Extract all the data from the fact string
	type = extractDataFromString(factString, "type");
	context = extractDataFromString(factString, "context");
	value = extractDataFromString(factString, "value");
}

EXSFact::EXSFact(const char* type, const char* context, const char* value)
{
	this->type = StringUtil::Strcpy(type);
	this->context = StringUtil::Strcpy(context);
	this->value = StringUtil::Strcpy(value);
}

EXSFact::EXSFact(const EXSFact& other)
{
	type = StringUtil::Strcpy(other.type);
	context = StringUtil::Strcpy(other.context);
	value = StringUtil::Strcpy(other.value);
}

EXSFact::~EXSFact()
{
	delete[] type;
	delete[] context;
	delete[] value;
}

char* EXSFact::GetType()
{
	return type;
}

char* EXSFact::GetContext()
{
	return context;
}

char* EXSFact::GetValue()
{
	return value;
}

void EXSFact::SetValue(const char* value)
{
	delete[] this->value;

	this->value = StringUtil::Strcpy(value);
}

char* EXSFact::GetFactString()
{
	if (!ContainsValidData())
	{
		return StringUtil::Strcpy("");
	}

	std::string result = "(state (type " + std::string(type) + ") (value " + std::string(value) + ") (context " + std::string(context) + "))";

	return StringUtil::Strcpy(result.c_str());
}

bool EXSFact::ContainsValidData()
{
	return type && context && value;
}

char* EXSFact::extractDataFromString(const char* factString, const char* dataField)
{
	bool found;

	// Look for "([dataField]"
	char* dataFieldWithBracket = StringUtil::Strcat("(", dataField);
	size_t dataFieldPosition = StringUtil::Strpos(factString, dataFieldWithBracket, &found);

	delete[] dataFieldWithBracket;

	if (!found)
	{
		return NULL;
	}

	// Move the position to "[dataField]"
	dataFieldPosition++;

	// Look for the closing bracket
	size_t closingBracketPosition = dataFieldPosition + StringUtil::Strpos(&factString[dataFieldPosition], ")", &found);

	if (!found)
	{
		return NULL;
	}

	// Extract the value
	size_t dataStartPosition = dataFieldPosition + StringUtil::Strlen(dataField) + 1;
	size_t dataLength = closingBracketPosition - dataStartPosition;

	return StringUtil::Strcpy(&factString[dataStartPosition], dataLength);
}