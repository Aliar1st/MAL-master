#pragma once

#include <stdint.h>
#include "Connection.h"
#include "SerialPortAsynchronousOperation.h"
#include "Threading/Timer.h"

#ifdef _WIN32

#include <windows.h>

typedef DWORD baudrate_t;

#elif __linux__

#include <termios.h>

typedef unsigned int baudrate_t;

#endif // __linux__

/**
 * Enumerates all possible return values of the functions of CSerialPort.
 */
enum : Connection_Return
{
	SERIAL_PORT_RETURN_ERROR_COULD_NOT_OPEN_PORT = CONNECTION_RETURN_AMOUNT,
	SERIAL_PORT_RETURN_ERROR_COULD_NOT_GET_PORTS_STATE,
	SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_STATE,
	SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_TIMEOUTS
};

/**
 * @brief	This class represents a asynchronous serial port connection object to send and
 * 			receive bytes. It supports Windows and Linux.
 */
class SerialPort : public Connection
{
	friend class SerialPortAsynchronousOperation;

private:

#ifdef __linux__
	constexpr static const char SERIAL_PORT_NAME_PREFIX[] = "/dev/";
#endif // __linux__

#ifdef _WIN32
#ifdef UNICODE
	WCHAR* internName = nullptr;
#endif // UNICODE

	HANDLE internPort;
#elif __linux__
	char* internName = nullptr;

	int internPort;
#endif // __linux__

	char* name = nullptr;

	/**
	 * @brief	Assigns the serial port's name according to its naming scheme.
	 *
	 * @param[in]	name	The desired name of the serial port.
	 */
	void assignSerialPortName(const char* name);

#ifdef __linux__
	/**
	 * @brief	Converts a raw baud rate to Linux' baud rate
	 *
	 * @param		rawBaudRate		Baud rate to convert
	 * @param[out]	validBaudRate	True if the provided baud rate is valid; False otherwise
	 *
	 * @return	Linux baud rate;
	 */
	speed_t rawBaudRateToLinuxBaudRate(baudrate_t rawBaudRate, bool* validBaudRate);
#endif //__linux__

protected:

	virtual void reset() override;

public:

	/**
	 * @brief	The constructor of SerialPort.
	 */
	SerialPort();

	/**
	 * @brief	The destructor of SerialPort.
	 */
	virtual ~SerialPort() override;

	/**
	 * @brief	Opens the serial port with the given name and baud rate. No parity bits are used and there is one stop bit.
	 *
	 * @param[in]	name		The name of the port to open.
	 * @param 		baudRate	The baud rate to use.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The serial port is already opened.
	 *			SERIAL_PORT_RETURN_ERROR_COULD_NOT_GET_PORTS_STATE		Could not get the port's current state.
	 *			SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_STATE		Could not set the port's state.
	 *			SERIAL_PORT_RETURN_ERROR_COULD_NOT_SET_PORTS_TIMEOUTS	Could not set the port's timeouts. (Windows only)
	 *			SERIAL_PORT_RETURN_ERROR_COULD_NOT_OPEN_PORT			Could not open the serial port.
	 */
	Connection_Return Open(const char* name, baudrate_t baudRate);

	Connection_Return Close() override;

	virtual SerialPortAsynchronousOperation* CreateAsynchronousOperation() override;

	/**
	 * @brief	Returns the name of the serial port.
	 *
	 * @return	The name of the serial port. NULL if the serial port is not opened.
	 */
	char* GetName();
};