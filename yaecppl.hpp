#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <time.h>

#ifndef YAECPPL_H
#define YAECPPL_H

#define YAECPPL_STATUS_SUCCESS 100
#define YAECPPL_STATUS_FAILURE 101

#define YAECPPL_LEVEL_INFO    200 
#define YAECPPL_LEVEL_DATA    201 
#define YAECPPL_LEVEL_WARNING 202
#define YAECPPL_LEVEL_ERROR   203 
#define YAECPPL_LEVEL_FATAL   204 

#define YAECPPL_TARGET_STDOUT  300
#define YAECPPL_TARGET_STDERR  301
#define YAECPPL_TARGET_SQUELCH 302
#define	YAECPPL_TARGET_FILE    303 

#define YAECPPL_VERIFY_PEDANTIC false
// if set to true and verify() fails, many functions will return YAECPPL_STATUS_FAILURE
// otherwise functions will attempt to continue, but may crash

// if this is set to true, all log messages will include a timestamp
#define YAECPPL_TIMESTAMP true

// if this is set to true, changes to the log file will be appended, without overwriting existing contents
#define YAECPPL_FILE_APPEND true
class logger
{
	public: 
		logger(std::string); // constructor
		int commit(int = YAECPPL_LEVEL_INFO); // commit current logstream to it's target 
		int verify(); // verify publicly writeable values are withing acceptable bounds
		int logData(std::string, int); // log data in a nice format automatically
		int logData(std::string, float); // overloaded to work with many variable types
		int logData(std::string, double);
		int logData(std::string, bool);
		int logData(std::string, std::string);
		int logMessage(std::string, int = YAECPPL_LEVEL_INFO); // log a string without using logstream
		std::stringstream logstream; // the log stream itself 
		std::vector<int> defaultLogTargets; // default log location(s), should be one or more of YAECPPL_TARGET declared above
		int errorLogThreshold; // anything with a loglevel equal to or great than this gets sent to errorLogTarget
		std::vector<int> errorLogTargets;  // target(s) of error logs, should be one or more of YAECPPL_TARGET declared above
		int squelchLogThreshold; // anything lower than this log level will be squelched 
		std::vector<int> squelchLogTargets; // target of squelched logs, should usually be set to YAECPPL_TARGET_SQUELCH
	private: 
		bool verifyVector(std::vector<int>&, std::vector<int>&);
		std::string generateTimestamp(); // generates a current timestamp
		bool vectorContains(std::vector<int>&, int); // check if a vector contains a particular value
		int logToTarget(std::string, int); // send a log message to a particular target
		std::string logFileName;
		bool logFileNotWriteable; // flipped to true if the log file cannot be opened
		std::string logSeperator; // separate each filed in the log message
		std::vector<std::string> logLevelTags; // list of valid tags for log levels 
};

#endif 