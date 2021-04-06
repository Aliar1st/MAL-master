#include "Logger.h"

#include "Utilities/StringUtil.h"

Logger::Logger()
{
	reset();
}

Logger::~Logger()
{
	// Stop the logger
	Stop();
}

void Logger::Start(Logger_Mode loggerMode)
{
	// Make this function thread safe
	std::unique_lock<std::mutex> uniqueLoggingLock(loggingMutex);

	// Check if already started
	if (isStarted)
	{
		return;
	}

	isStarted = true;
	this->loggerMode = loggerMode;
	logFileName = StringUtil::GetTimeStamp("log_%y-%m-%d_%H-%M-%S.log");
}

void Logger::Stop()
{
	// Make this function thread safe
	std::unique_lock<std::mutex> uniqueLoggingLock(loggingMutex);

	// Check if already stopped
	if (!isStarted)
	{
		return;
	}

	reset();
}

Logger& Logger::StartLogging(Logger_Mode loggingMode)
{
	// Prevent two simultaneous loggings
	loggingMutex.lock();
	isLogging = (isStarted && loggingMode <= loggerMode && loggingMode != Logger_Mode::NONE);

	if (!isLogging)
	{
		return *this;
	}

	// Create the log file if necessary
	if (!fileStream.is_open())
	{
		fileStream.open(logFileName, std::ofstream::out);
	}

	// Log the meta data
	logTimeStamp();
	logLoggingMode(loggingMode);

	return *this;
}

Logger& Logger::EndLogging()
{
	// Add a new line
	*this << std::endl;

	isLogging = false;

	// Unlock the mutex to unlock logging other data
	loggingMutex.unlock();

	return *this;
}

Logger& Logger::operator<<(const Logger& manipulator)
{
	return *this;
}

Logger& Logger::operator<<(Logger& (*manipulator)(Logger&))
{
	manipulator(*this);

	return *this;
}

bool Logger::IsLogging()
{
	return isLogging;
}

Logger& Logger::Endl(Logger& logger)
{
	if (logger.isLogging)
	{
		// Line break and indent (account for the time stamp and the logging mode)
		logger << "\n" << "                    ";
	}

	return logger;
}

Logger& Logger::Indent(Logger& logger)
{
	if (logger.isLogging)
	{
		logger << "    ";
	}

	return logger;
}

void Logger::logTimeStamp()
{
	const char* timeStamp = StringUtil::GetTimeStamp("[%H:%M:%S] ");

	*this << timeStamp;
	delete[] timeStamp;
}

void Logger::logLoggingMode(Logger_Mode loggingMode)
{
	switch (loggingMode)
	{
		case Logger_Mode::ERRORS:
		{
			*this << "ERROR:   ";
			break;
		}

		case Logger_Mode::STATUS:
		{
			*this << "STATUS:  ";
			break;
		}

		case Logger_Mode::INFO:
		{
			*this << "INFO:    ";
			break;
		}
	}
}

Logger& Logger::operator<<(std::ostream& (*manipulator)(std::ostream&))
{
	if (isLogging)
	{
		manipulator(fileStream);
	}

	return *this;
}

void Logger::reset()
{
	isStarted = false;
	loggerMode = Logger_Mode::NONE;
	isLogging = false;

	delete[] logFileName;
	logFileName = nullptr;

	if (fileStream.is_open())
	{
		fileStream.close();
	}
}