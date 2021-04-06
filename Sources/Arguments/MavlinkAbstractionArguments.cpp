#include "MavlinkAbstractionArguments.h"

#include "Utilities/StringUtil.h"

MavlinkAbstractionArguments::~MavlinkAbstractionArguments()
{
	delete[] serialPortName;
}

void MavlinkAbstractionArguments::SetSerialPortName(const char* serialPortName)
{
	delete[] this->serialPortName;
	this->serialPortName = StringUtil::Strcpy(serialPortName);
}