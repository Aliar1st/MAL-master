#include "SerialPort.h"

#include "Utilities/ConversionUtil.h"
#include "Utilities/StringUtil.h"

#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>

#endif // __linux__

SerialPort::SerialPort()
{
	reset();
}

SerialPort::~SerialPort()
{
	if (isOpened)
	{
		Close();
	}
}

Connection_Return SerialPort::Open(const char* name, baudrate_t baudRate)
{
	// Check if the port is already opened
	if (isOpened)
	{
		return CONNECTION_RETURN_ERROR_ALREADY_OPENED;
	}

	// Assign the serial port's name
	assignSerialPortName(name);

#ifdef _WIN32
	// Open the port
	internPort = CreateFile(
#ifdef UNICODE
		internName,                     // Port name
#else
		name,                           // Port name
#endif // UNICODE
		GENERIC_WRITE | GENERIC_READ,   // Access mode
		0,                              // Share mode
		0,                              // Security attributes
		OPEN_EXISTING,                  // Creating disposition
		FILE_FLAG_OVERLAPPED,           // Flags and attributes
		0                               // Template file
	);

	// Check for errors
	if (internPort == INVALID_HANDLE_VALUE)
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_OPEN_PORT;
	}

	/**
	 * Get the port's parameters
	 */
	DCB portParameters;

	portParameters.DCBlength = sizeof(portParameters);
	if (!GetCommState(internPort, &portParameters))
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_GET_PORTS_STATE;
	}

	/**
	 * Set the port's baud rate, byte size, stop bits and parity
	 */
	portParameters.BaudRate = baudRate;
	portParameters.ByteSize = 8;
	portParameters.StopBits = ONESTOPBIT;
	portParameters.Parity = NOPARITY;
	if (!SetCommState(internPort, &portParameters))
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_STATE;
	}

	/**
	 * Set the port's timeouts
	 */
	COMMTIMEOUTS timeouts;

	// Setup reading to time out if there is no new data for 5ms
	timeouts.ReadIntervalTimeout = 5;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;

	// Setup writing to not time out
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(internPort, &timeouts))
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_TIMEOUTS;
	}
#elif __linux__
	internPort = open(internName, O_RDWR | O_NOCTTY);

	// Check for errors
	if (internPort == -1)
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_OPEN_PORT;
	}

	/**
	 * Get the port's parameters
	 */
	struct termios portParameters;

	if (tcgetattr(internPort, &portParameters))
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_GET_PORTS_STATE;
	}

	// Set raw mode
	cfmakeraw(&portParameters);

	// Allow reading
	portParameters.c_cflag |= CREAD;

	// Ignore modem status line
	portParameters.c_cflag |= CLOCAL;

	// 1 stop bit
	portParameters.c_cflag &= ~CSTOPB;

	// Disable hardware flow control
	portParameters.c_cflag &= ~CRTSCTS;

	// Set to not timeout unless there is data
	portParameters.c_cc[VMIN] = 1;
	portParameters.c_cc[VTIME] = 1;

	// Convert baud rate
	bool validBaudRate;
	speed_t linuxBaudRate = rawBaudRateToLinuxBaudRate(baudRate, &validBaudRate);

	if (!validBaudRate)
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_STATE;
	}

	// Set baud rate
	cfsetispeed(&portParameters, linuxBaudRate);
	cfsetospeed(&portParameters, linuxBaudRate);

	// Set parameters
	if (tcsetattr(internPort, TCSANOW, &portParameters))
	{
		return SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_STATE;
	}
#endif // __linux__

	// No error occurred
	isOpened = true;

	return CONNECTION_RETURN_SUCCESS;
}

Connection_Return SerialPort::Close()
{
	// Check if the port is already opened
	if (!isOpened)
	{
		return CONNECTION_RETURN_ERROR_NOT_OPENED;
	}

#ifdef _WIN32
	// Close the handle
	CloseHandle(internPort);
#elif __linux__
	close(internPort);
#endif // __linux__

	reset();

	return CONNECTION_RETURN_SUCCESS;
}

SerialPortAsynchronousOperation* SerialPort::CreateAsynchronousOperation()
{
	return new SerialPortAsynchronousOperation(this);
}

char* SerialPort::GetName()
{
	return name;
}

void SerialPort::assignSerialPortName(const char* name)
{
	// Assign the name(s)
	this->name = StringUtil::Strcpy(name);

#ifdef _WIN32
#ifdef UNICODE
	internName = ConversionUtil::StrToWStr(name);
#endif // UNICODE
#elif __linux__
	internName = StringUtil::Strcat(SERIAL_PORT_NAME_PREFIX, name);
#endif // __linux__
}

void SerialPort::reset()
{
	// Free the memory allocated for the name(s)
	delete[] name;
	name = 0;

#if defined(__linux__) || defined(UNICODE)
	delete[] internName;
	internName = 0;
#endif // defined(__linux__) || defined(UNICODE)

	Connection::reset();
}

#ifdef __linux__

speed_t SerialPort::rawBaudRateToLinuxBaudRate(baudrate_t rawBaudRate, bool* validBaudRate)
{
	*validBaudRate = true;

	switch (rawBaudRate)
	{
		case 0:

			return B0;

		case 50:

			return B50;

		case 75:

			return B75;

		case 110:

			return B110;

		case 134:

			return B134;

		case 150:

			return B150;

		case 200:

			return B200;

		case 300:

			return B300;

		case 600:

			return B600;

		case 1200:

			return B1200;

		case 1800:

			return B1800;

		case 2400:

			return B2400;

		case 4800:

			return B4800;

		case 9600:

			return B9600;

		case 19200:

			return B19200;

		case 38400:

			return B38400;

		case 57600:

			return B57600;

		case 115200:

			return B115200;

		case 230400:

			return B230400;

		case 460800:

			return B460800;

		case 500000:

			return B500000;

		case 576000:

			return B576000;

		case 921600:

			return B921600;

		case 1000000:

			return B1000000;

		case 1152000:

			return B1152000;

		case 1500000:

			return B1500000;

		case 2000000:

			return B2000000;

		case 2500000:

			return B2500000;

		case 3000000:

			return B3000000;

		case 3500000:

			return B3500000;

		case 4000000:

			return B4000000;
	}

	*validBaudRate = false;

	return B0;
}

#endif //__linux__