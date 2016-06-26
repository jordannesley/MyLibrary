/**
*  @file    DebugLogger.h
*  @author  Jordan Nesley
**/

#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

/*
A Class that allows for logging certian messages to the command line when in debug mode

Has a singleton design pattern
*/

/** An Enumeration for the different types of messages to log
*/
enum MessageLoggerType
{
	Information,
	Warning,
	Error
};


class DebugLogger {

	private:
		// private constructor
		DebugLogger() {};

		// These lines override default class properties that would allow copies of the singleton class to be made
		DebugLogger(DebugLogger const&); // Don't implement. Override the defualt copy constructor
		void operator=(DebugLogger const&); // Don't implement. override the default assignment operator

		// The debug flag (True => log messages, False => don't log messages)
		static bool m_debugFlag;

	public:

		static DebugLogger& getInstance();

		DebugLogger(bool aDebugFlag);
		void setDebugFlag(bool aDebugFlag);
		void logMessage(MessageLoggerType aMessageType, const char* aCallingFunctionName, const char* aMessage);
};

#endif