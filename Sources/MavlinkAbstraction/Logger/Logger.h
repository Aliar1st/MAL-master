#pragma once

#include <fstream>
#include <mutex>
#include "Logger_Mode.h"

/**
 * @brief	A thread safe logger used to log data from the Mavlink Abstraction
 */
class Logger
{
private:

	bool isStarted;
	Logger_Mode loggerMode;
	const char* logFileName = nullptr;

	std::mutex loggingMutex;
	bool isLogging;
	std::ofstream fileStream;

	/**
	 * @brief	Logs the current time stamp
	 */
	void logTimeStamp();

	/**
	 * @brief	Logs the logging mode
	 *
	 * @param	loggingMode		The logging mode to be log
	 */
	void logLoggingMode(Logger_Mode loggingMode);

	/**
	 * @brief	Logs an ostream manipulator
	 *
	 * @param[in]	manipulator	Manipulator to be logged
	 *
	 * @return	This logger
	 */
	Logger& operator<<(std::ostream& (*manipulator)(std::ostream&));

	/**
	 * @brief	Resets the logger to its original state
	 */
	void reset();

public:

	/**
	 * @brief	The constructor of Logger
	 */
	Logger();

	/**
	 * @brief	The destructor of Logger
	 */
	~Logger();

	/**
	 * @brief	Starts the logger
	 *
	 * @param	loggerMode	The logger mode to use
	 */
	void Start(Logger_Mode loggerMode);

	/**
	 * @brief	Stops the logger
	 */
	void Stop();

	/**
	 * @brief	Starts logging. Call this before passing any data to the logger.
	 *
	 * @param	loggingMode		The logging mode used for the data to be logged
	 *							The data to be logged will only be logged if this logging mode is less or equal to the logger mode and if it is unequal to Logger_Mode::NONE
	 *
	 * @return	This logger
	 */
	Logger& StartLogging(Logger_Mode loggingMode);

	/**
	 * @brief	Ends logging. Call this after passing any data to the logger
	 *
	 * @return	This logger
	 */
	Logger& EndLogging();

	/**
	 * @brief	Logs a Logger manipulator. This is used to conveniently pass StartLogging and EndLogging.
	 *
	 * @param[in]	manipulator	Manipulator to be logged
	 *
	 * @return	This logger
	 */
	Logger& operator<<(const Logger& manipulator);

	/**
	 * @brief	Logs a Logger manipulator. This is used to pass Endl.
	 *
	 * @param[in]	manipulator	Manipulator to be logged
	 *
	 * @return	This logger
	 */
	Logger& operator<<(Logger& (*manipulator)(Logger&));

	/**
	 * @brief	Logs any data. Do not pass a line break using this. Instead, use Logger::Endl.
	 *
	 * @param[in]	data	Data to be logged
	 *
	 * @return	This logger
	 *
	 * @tparam	The type of the data to be logged.
	 */
	template<class T>
	Logger& operator<<(const T& data)
	{
		if (isLogging)
		{
			fileStream << data;
		}

		return *this;
	}

	/**
	 * @brief	Returns whether the logger is currently logging. This is useful to prevent overhead that results from calculating the data to log.
	 *			Use this only in between StartLogging(...) and EndLogging().
	 *
	 * @return	True if the logger is logging; False otherwise.
	 */
	bool IsLogging();

	/**
	 * @brief	Adds a line break to the logger
	 *
	 * @param[in]	logger	Logger to perform the line break on
	 *
	 * @return	The logger
	 */
	static Logger& Endl(Logger& logger);

	/**
	 * @brief	Adds an indent to the logger
	 *
	 * @param[in]	logger	Logger to perform the indent on
	 *
	 * @return	The logger
	 */
	static Logger& Indent(Logger& logger);
};