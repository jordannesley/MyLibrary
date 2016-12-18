/**
*  @file    DebugLogger.cpp
*  @author  Jordan Nesley
**/

#include "DebugLogger.h"

static const char* WARNING_STRING = "WARNING";
static const char* ERROR_STRING = "ERROR";
static const char* INFORMATION_STRING = "INFORMATION";
static const char* UNKOWN_STRING = "UNKOWN";

bool DebugLogger::m_debugFlag = false;

/** Returns the only instance of the DebugLogger
*/
DebugLogger& DebugLogger::getInstance()
{
	static DebugLogger Instance;
	return Instance;
}

/** Sets the debug flag of the DebugLogger
* @param aDebugFlag The debug flag
*/
void DebugLogger::setDebugFlag(bool aDebugFlag)
{
	m_debugFlag = aDebugFlag;

	char* lMessage;

	if (m_debugFlag)
	{
		lMessage = (char*)"Debug Flag Set True";
	}
	else
	{
		lMessage = (char*)"Debug Flag Set False";
	}

	DebugLogger::logMessage(MessageLoggerType::Information, __FUNCTION__, lMessage);

}

/** Logs a message to the command line
* @param aMessageType The type of message to display
* @param aMessage the Message to display
*/
void DebugLogger::logMessage(MessageLoggerType aMessageType, const char* aCallingFunctionName, const char* aMessage)
{
	const char* lMessage;

	if (m_debugFlag)
	{
		switch (aMessageType)
		{
		case MessageLoggerType::Warning:
			lMessage = WARNING_STRING;
			break;
		case MessageLoggerType::Error:
			lMessage = ERROR_STRING;
			break;
		case MessageLoggerType::Information:
			lMessage = INFORMATION_STRING;
			break;
		default:
			lMessage = UNKOWN_STRING;
			break;
		}

		// output the message
		std::cout << __FUNCTION__ << " : " << lMessage << " : " << aCallingFunctionName << " : " << aMessage << std::endl;

	};
}
