#include "yaecppl.hpp"


logger::logger(std::string targetFileName)
{
	logFileName = targetFileName; // set name of log file 
	// make sure we can open the log file
	std::ofstream logFileStream;

	if(YAECPPL_FILE_APPEND)
	{
		logFileStream.open(logFileName.c_str(), std::ios_base::app);
	}
	else
	{
		logFileStream.open(logFileName.c_str());
	}

	if (logFileStream.is_open())
	{
		logFileStream.close();
		logFileNotWriteable = false; 
	}
	else
	{
		std::cerr << "WARNING: yaecppl could not write to \"" << logFileName << "\"" << ". logs targeting this file will be squelched instead" << std::endl;
		logFileNotWriteable = true;
	}

	// setup default targets and thresholds
	errorLogThreshold   = YAECPPL_LEVEL_WARNING; 
	squelchLogThreshold = YAECPPL_LEVEL_INFO; //nothing squelched
	defaultLogTargets.push_back(YAECPPL_TARGET_STDOUT );
	errorLogTargets.push_back  (YAECPPL_TARGET_STDERR );
	squelchLogTargets.push_back(YAECPPL_TARGET_SQUELCH);

	logSeperator = '|';
	logLevelTags.push_back("info" );
	logLevelTags.push_back("data" );
	logLevelTags.push_back("warn" );
	logLevelTags.push_back("error");
	logLevelTags.push_back("fatal");
}

std::string logger::generateTimestamp()
{
	// generates a current timestamp in YYY:MM:DD:HH:mm:ss format 
	// returns the same as a string 
	time_t currentTime = time(0);
	struct tm timeStructure; 
	char timestamp[80]; 
	timeStructure = *localtime(&currentTime); 
	strftime(timestamp, sizeof(timestamp), "%Y:%m:%d:%X", &timeStructure);
	return timestamp; 

}

bool logger::verifyVector(std::vector<int>& vectorToVerify, std::vector<int>& validValues)
{
	// verifies that each index of vectorToVerify matches at least one index of validValues 
	// if so, returns true, otherwise returns false
	for (int i = 0; i < vectorToVerify.size(); i++)
	{
		bool passed = false; 
		for (int j = 0; j < validValues.size(); j++)
		{
			if (validValues[j] == vectorToVerify[i])
			{
				passed = true; 
			}
		}
		if (!passed)
		{
			return false; 
		}
	}
	return true; 
}

int logger::verify()
{
	// this function verifies that all publicly modifiable variables are within acceptable bounds, namely
	// defaultLogTargets
	// errorLogThreshold
	// errorLogTargets
	// squelchLogThreshold
	// squelchLogTargets 

	// if all variables are within acceptable bounds, returns YAECPPL_STATUS_SUCCESS
	// if not, returns YAECPPL_STATUS_FAILURE

	int status = YAECPPL_STATUS_SUCCESS; 

	// targets 
	std::vector<int> validTargets;
	validTargets.push_back(YAECPPL_TARGET_STDOUT );
	validTargets.push_back(YAECPPL_TARGET_STDERR );
	validTargets.push_back(YAECPPL_TARGET_SQUELCH);
	validTargets.push_back(YAECPPL_TARGET_FILE   );

	if (!verifyVector(defaultLogTargets, validTargets))
	{
		std::cerr << "WARNING: yaecppl defaultLogTargets contains an invalid target" << std::endl;
		status = YAECPPL_STATUS_FAILURE;
	}
	if (!verifyVector(errorLogTargets, validTargets))
	{
		std::cerr << "WARNING: yaecppl errorLogTargets contains an invalid target" << std::endl;
		status = YAECPPL_STATUS_FAILURE;
	}
	if (!verifyVector(squelchLogTargets, validTargets))
	{
		std::cerr << "WARNING: yaecppl squelchLogTargets contains an invalid target" << std::endl;
		status = YAECPPL_STATUS_FAILURE;
	}

	// levels 
	std::vector<int> validLevels; 
	validLevels.push_back(YAECPPL_LEVEL_INFO   );
	validLevels.push_back(YAECPPL_LEVEL_DATA   );
	validLevels.push_back(YAECPPL_LEVEL_WARNING);
	validLevels.push_back(YAECPPL_LEVEL_ERROR  );
	validLevels.push_back(YAECPPL_LEVEL_FATAL  );

	bool errorLogThresholdPassed = false; 
	bool squelchLogThresholdPassed = false;
	for (int i = 0; i < validLevels.size(); i++)
	{
		if (errorLogThreshold == validLevels[i])
		{
			errorLogThresholdPassed = true;
		}
		if (squelchLogThreshold == validLevels[i])
		{
			squelchLogThresholdPassed = true; 
		}
	}

	if (!errorLogThresholdPassed)
	{
		std::cerr << "WARNING: yaecppl errorLogThreshold has invalid value: " << errorLogThreshold << std::endl;
		status = YAECPPL_STATUS_FAILURE;
	}
	if (!squelchLogThresholdPassed)
	{
		std::cerr << "WARNING: yaecppl squelchLogThreshold has invalid value: " << squelchLogThreshold << std::endl;
		status = YAECPPL_STATUS_FAILURE;
	}

	return status;
}

bool logger::vectorContains(std::vector<int>& vectorToCheck, int targetValue)
{
	// checks if vectorToCheck contains at least one instance of targetValue
	// if so, returns true, otherwise returns false
	for(int i = 0; i < vectorToCheck.size(); i++)
	{
		if (vectorToCheck[i] == targetValue)
		{
			return true;
		}
	}
	return false;
}

int logger::logData(std::string name, int data)
{
	// produces a log message with loglevel YAECPPL_LEVEL_DATA, and the contents "name=data", replacing name and data with their values 
	// on success returns YAECPPL_STATUS_SUCCESS, otherwise returns YAECPPL_STATUS_FAILURE 
	int functionStatus;  // make sure commit works okay 
	logstream << name << "=" << data; 
	functionStatus = commit(YAECPPL_LEVEL_DATA); 
	if(functionStatus != YAECPPL_STATUS_SUCCESS)
	{
		std::cerr << "WARNING: yaecppl logData() encountered an error while committing log message" << std::endl; 
		return YAECPPL_STATUS_FAILURE;
	}
	return YAECPPL_STATUS_SUCCESS; 
}

int logger::logData(std::string name, float data)
{
	// produces a log message with loglevel YAECPPL_LEVEL_DATA, and the contents "name=data", replacing name and data with their values 
	// on success returns YAECPPL_STATUS_SUCCESS, otherwise returns YAECPPL_STATUS_FAILURE 
	int functionStatus;  // make sure commit works okay 
	logstream << name << "=" << data; 
	functionStatus = commit(YAECPPL_LEVEL_DATA); 
	if(functionStatus != YAECPPL_STATUS_SUCCESS)
	{
		std::cerr << "WARNING: yaecppl logData() encountered an error while committing log message" << std::endl; 
		return YAECPPL_STATUS_FAILURE;
	}
	return YAECPPL_STATUS_SUCCESS; 
}


int logger::logData(std::string name, double data)
{
	// produces a log message with loglevel YAECPPL_LEVEL_DATA, and the contents "name=data", replacing name and data with their values 
	// on success returns YAECPPL_STATUS_SUCCESS, otherwise returns YAECPPL_STATUS_FAILURE 
	int functionStatus;  // make sure commit works okay 
	logstream << name << "=" << data; 
	functionStatus = commit(YAECPPL_LEVEL_DATA); 
	if(functionStatus != YAECPPL_STATUS_SUCCESS)
	{
		std::cerr << "WARNING: yaecppl logData() encountered an error while committing log message" << std::endl; 
		return YAECPPL_STATUS_FAILURE;
	}
	return YAECPPL_STATUS_SUCCESS; 
}

int logger::logData(std::string name, bool data)
{
	// produces a log message with loglevel YAECPPL_LEVEL_DATA, and the contents "name=data", replacing name and data with their values 
	// on success returns YAECPPL_STATUS_SUCCESS, otherwise returns YAECPPL_STATUS_FAILURE 
	int functionStatus;  // make sure commit works okay 
	logstream << name << "=" << data; 
	functionStatus = commit(YAECPPL_LEVEL_DATA); 
	if(functionStatus != YAECPPL_STATUS_SUCCESS)
	{
		std::cerr << "WARNING: yaecppl logData() encountered an error while committing log message" << std::endl; 
		return YAECPPL_STATUS_FAILURE;
	}
	return YAECPPL_STATUS_SUCCESS; 
}

int logger::logData(std::string name, std::string data)
{
	// produces a log message with loglevel YAECPPL_LEVEL_DATA, and the contents "name=data", replacing name and data with their values 
	// on success returns YAECPPL_STATUS_SUCCESS, otherwise returns YAECPPL_STATUS_FAILURE 
	int functionStatus;  // make sure commit works okay 
	logstream << name << "=" << data; 
	functionStatus = commit(YAECPPL_LEVEL_DATA); 
	if(functionStatus != YAECPPL_STATUS_SUCCESS)
	{
		std::cerr << "WARNING: yaecppl logData() encountered an error while committing log message" << std::endl; 
		return YAECPPL_STATUS_FAILURE;
	}
	return YAECPPL_STATUS_SUCCESS; 
}

int logger::logMessage(std::string message, int logLevel)
{
	// creates a log message with the contents of message and level logLevel 
	// returns YAECPPL_STATUS_SUCCESS on success, and YAECPPL_STATUS_FAILURE on failure
	int functionStatus;
	logstream << message; 
	functionStatus = commit(logLevel); 
	if(functionStatus != YAECPPL_STATUS_SUCCESS)
	{
		std::cerr << "WARNING: yaecppl logMessage() encountered an error while commit log message" << std::endl;
		return YAECPPL_STATUS_FAILURE;
	}
	return YAECPPL_STATUS_SUCCESS; 
}

int logger::logToTarget(std::string message, int target)
{
	// message should be the message to be logged 
	// target should be the target of the message (one of YAECPPL_TARGET defined in yaecppl.hpp)
	// returns YAECPPL_STATUS_FAILURE if an error is encountered, otherwise returns YAECPPL_STATUS_SUCCESS
	if(target == YAECPPL_TARGET_STDOUT)
	{
		std::cout << message << std::endl;
	}
	else if(target == YAECPPL_TARGET_STDERR)
	{
		std::cerr << message << std::endl; 
	}
	else if(target == YAECPPL_TARGET_FILE)
	{
		if(logFileNotWriteable)
		{
			logToTarget(message, YAECPPL_TARGET_SQUELCH);
			std::cerr << "WARNING: yaecppl logToTarget() squelched a log message because \"" << logFileName << "\" was not writeable" << std::endl; 
			return YAECPPL_STATUS_FAILURE; 
		}

		std::ofstream targetFileStream; 
		targetFileStream.open(logFileName.c_str(), std::ios_base::app);

		if (targetFileStream.is_open())
		{
			targetFileStream << message << std::endl; 
		}
		else
		{
			std::cerr << "WARNING: yaecppl logToTarget() squelched a log message because \"" << logFileName << "\" used to be writeable, but is not anymore" << std::endl; 
			logToTarget(message, YAECPPL_TARGET_SQUELCH);
			return YAECPPL_STATUS_FAILURE; 
		}
		targetFileStream.close();
	}
	else if(target == YAECPPL_TARGET_SQUELCH)
	{
		// do nothing 
	}
	else
	{
		std::cerr << "WARNING: yaecppl logToTarget() was passed an invalid target: " << target << " and will squelch the message instead" << std::endl;
		logToTarget(message, YAECPPL_TARGET_SQUELCH); 
		return YAECPPL_STATUS_FAILURE;  
	}
	return YAECPPL_STATUS_SUCCESS; 
}

int logger::commit(int logLevel)
{
	// commits the current contents of logstream to it's target
	// int logLevel should be the log level of the current log, should be one of YAECPPL_LOG_LEVEL declared in yaecppl.hpp
	// if logLevel is not given, it will default to YAECPPL_LEVEL_INFO

	std::string message = ""; // the message to be logged
	int functionStatus; // this will be used to make sure our functions are ok 

	if(YAECPPL_TIMESTAMP)
	{
		message = generateTimestamp();
		message.append(logSeperator); 
	}

	if(logLevel - YAECPPL_LEVEL_INFO >= logLevelTags.size())
	{
		std::cerr << "WARNING: yaecppl logLevelTags does not contain requested tag, reverting to info instead" << std::endl; 
		commit(YAECPPL_LEVEL_INFO); 
		return YAECPPL_STATUS_FAILURE;
	}
	message.append(logLevelTags[logLevel - YAECPPL_LEVEL_INFO]); // this will break if the levels are not sequential ints with INFO as the lowest level
	message.append(logSeperator); 

	message.append(logstream.str()); // load the log stream into message
	logstream.str(""); // clear the log stream

	if (verify() != YAECPPL_STATUS_SUCCESS) // make sure our targets and levels are okay 
	{
		if (YAECPPL_VERIFY_PEDANTIC)
		{
			std::cerr << "ERROR: yaecppl verify() detected one or more errors, refusing to commit" << std::endl; 
			return YAECPPL_STATUS_FAILURE; 
		} 
		else
		{
			std::cerr << "WARNING: yaecppl verify() detected one or more errors, commit may crash or produce unexpected results" << std::endl;
		}
	}

	if(logLevel >= squelchLogThreshold && logLevel < errorLogThreshold)
	{
		for(int i = 0; i < defaultLogTargets.size(); i++)
		{
			functionStatus = logToTarget(message, defaultLogTargets[i]); // log to each specified target
			if(functionStatus == YAECPPL_STATUS_FAILURE)
			{
				std::cerr << "WARNING: logToTarget() encountered one or more errors, commit may produce unexpected results" << std::endl; 
				return YAECPPL_STATUS_FAILURE;
			}
		}
	}
	else if(logLevel < squelchLogThreshold)
	{
		for(int i = 0; i < squelchLogTargets.size(); i++)
		{
			functionStatus = logToTarget(message, squelchLogTargets[i]); // log to each specified target
			if(functionStatus == YAECPPL_STATUS_FAILURE)
			{
				std::cerr << "WARNING: logToTarget() encountered one or more errors, commit may produce unexpected results" << std::endl; 
				return YAECPPL_STATUS_FAILURE;
			}
		}
	}
	else if(logLevel >= errorLogThreshold)
	{
		for(int i = 0; i < errorLogTargets.size(); i++)
		{
			functionStatus = logToTarget(message, errorLogTargets[i]); // log to each specified target
			if(functionStatus == YAECPPL_STATUS_FAILURE)
			{
				std::cerr << "WARNING: logToTarget() encountered one or more errors, commit may produce unexpected results" << std::endl; 
				return YAECPPL_STATUS_FAILURE;
			}
		}
	}
	else
	{
		std::cerr << "WARNING: yaecppl commit() could not resolve message for log level: " << logLevel << " (this should never happen)" << std::endl; 
		return YAECPPL_STATUS_FAILURE; 
	}


	return YAECPPL_STATUS_SUCCESS;
}


