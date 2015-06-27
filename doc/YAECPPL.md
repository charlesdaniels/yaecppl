# YAECPPL Documentation 

## YAECPPL by example 
A copy of the below is included in the same directory as this file, for your convienience. Place it in the same directory as `yaecppl.hpp` and `yaecppl.o`, then compile with `g++ -o yaecppl-example yaecppl-example.cpp yaecppl.o`. 
```cpp
#include <iostream> 
#include "yaecppl.hpp" // include yaecppl in our project 

int main()
{

	logger log("example.log"); // instantiate a logger instance 


	log.logstream << "this is info"; // write "this is info" to the log stream
	log.commit(YAECPPL_LEVEL_INFO); // commit the current log stream, explicitly setting the log level to YAECPPL_LEVEL_INFO 

	log.logstream << "this is data";
	log.commit(YAECPPL_LEVEL_DATA);

	log.logstream << "this is a warning";
	log.commit(YAECPPL_LEVEL_WARNING);

	log.logstream << "this is an error";
	log.commit(YAECPPL_LEVEL_ERROR); 

	log.logstream << "this is a fatal error"; 
	log.commit(YAECPPL_LEVEL_FATAL); 

	log.logstream << "this will default to \"info\""; 
	log.commit(); // commit the current log stream without explicitly setting a log level. This will cause the log level to be set to YAECPPL_LEVEL_INFO

	// an example of how to check a YAECPPL function for errors
	int status; // we need somewhere to store the status returned 
	log.logstream << "this is an example of how to do error handling"; // put something in the log stream
	status = log.commit();  // commit the stream, setting status to commit()'s return value 
	if(status != YAECPPL_STATUS_SUCCESS) // check if it did not succeed 
	{
		std::cout << "ERROR: yaecppl encountered an error while committing log message" << std::endl;
	}

	int foo = 10;
	log.logData("foo", foo); // a logData() example 
	log.logMessage("that was an example of logData()"); //a log message example
	\\ Note that logMessage() does not require a log level. If one is not specified, it will default to YAECPPL_LEVEL_INFO
	float bar = 10.75;
	log.logData("bar",bar);
	log.logMessage("That was another example of logData(), this time with a float");

	log.defaultLogTargets.push_back(YAECPPL_TARGET_FILE); // add the log file as a target for normal log messages 
	log.logstream << "this message will also go a file";
	log.commit(); // now when we commit normal log messages, they will go to stdout, AND to the log file 

}
```
output: 
```
2015:06:27:15:09:54|info|this is info
2015:06:27:15:09:54|data|this is data
2015:06:27:15:09:54|warn|this is a warning
2015:06:27:15:09:54|error|this is an error
2015:06:27:15:09:54|fatal|this is a fatal error
2015:06:27:15:09:54|info|this will default to "info"
2015:06:27:15:09:54|info|this is an example of how to do error handling
2015:06:27:15:09:54|data|foo=10
2015:06:27:15:09:54|info|that was an example of logData()
2015:06:27:15:09:54|data|bar=10.75
2015:06:27:15:09:54|info|That was another example of logData(), this time with a float
2015:06:27:15:09:54|info|this message will also go a file
```

## Conventions
Yaecppl uses a number of internal conventions for ease of use and simplicity. It's core purpose is, naturally, to log information. Information can be logged to one of four (really three) destinations: 

* stdout
* stderr
* file
* squelch 

Stdout and stderr should be self-explanatory, and file refers to a log file, whose name is set when the logger instance is instantiated. Messages sent to squealch are simply ignored, and are not output or saved anywhere. Yaecppl, as mentioned in the readme, also has several "log levels", indicating the severity of the log message. They are as follows: 

* info
* data
* warning
* error
* fatal error 

While log level is relatively meaningless to yaecppl (eg. creating a message with the "fatal error" log level will not cause your program to terminate), log levels are used to dictate where your messages will be output or saved. There are two variables which control this, `errorLogThreshold` and `squelchLogThreshold`. Any message with a log level equal to or greater than `errorLogThreshold` will be sent to all error log targets, and any message with a log level lower than `squelchLogThreshold` will be sent to all squelch targets. All other log messages will be sent to all default targets. 

The target of a log message is dictated by one of three variables, each of which is an int vector containing a list of targets the message should be sent to. Squelched logs will be sent to each target in `squelchLogTargets` (usually only `YAECPPL_TARGET_SQUELCH`, but this can be overridden at runtime), normal logs will be sent to each target in `defaultLogTargets`. Error logs will be sent to each target in `errorLogTargets`. 

## Configuration
### Runtime 
Most of the configuration for yaecppl is possible during runtime. Each instance of logger contains a number of public variables used for this purpose. They are" 

* `logstream` (std::stringstream) - the content of `logstream` is sent to it's target when `commit()` is called
* `defaultLogTargets` (std::vector\<int>; default value is [YAECPPL_TARGET_STDOUT]) - specifies the default target(s) for log messages 
* `errorLogThreshold` (int; default value is YAECPPL_LEVEL_WARNING) - any log message with a log level greater or equal to `errorLogThreshold` is sent to all targets in `errorLogTargets` 
* `errorLogTargets` (std::vector\<int>; default value is [YAECPPL_TARGET_STDERR]) - species the target(s) of error logs 
* `squelchLogThreshold` (int; default value is YAECPPL_LEVEL_INFO (this means nothing is squelched)) - any log message with a log level less than `squelchLogThreshold` is sent to all targets in `squelchLogTargets`
* `squelchLogTargets` (std::vector\<int>; default value is [YAECPPL_TARGET_SQUELCH]) - specifies the target(s) of squelched logs (technically, you could have squelched logs go to different destinations if you wnated to) 

### Compile time 
Some configuration options can only be changed at compile time. These are all set in `yaecppl.hpp`, and are: 

* `YAECPPL_VERIFY_PEDANTIC` (default value is false) - if set to true, `commit()` will not process the log message at all if `verify()` encounters any errors. Otherwise, a warning will be printed if `verify()` encounters any errors
* `YAECPPL_TIMESTAMP` (default value is true) - if set to true, all messages will be preceded by timestamps, otherwise they will not
* `YAECPPL_FILE_APPEND` (default value is true) - if set to true, the log file will be opened in append mode, otherwise it will be overwritten each run

### Other constants 
Yaecppl also defines several other useful constants: 

* Status 
 * `YAECPPL_STATUS_SUCCESS` - when returned from a function, indicates it has succeeded 
 * `YAECPPL_STATUS_FAILURE` - when returned from a function, indicates it has failed 
* Log levels 
 * `YAECPPL_LEVEL_INFO` - used as an argument to indicate a log has the info log level 
 * `YAECPPL_LEVEL_DATA` - used as an argument to indicate a log has the data log level 
 * `YAECPPL_LEVEL_WARNING` - used as an argument to indicate a log has the warning log level 
 * `YAECPPL_LEVEL_ERROR` - used as an argument to indicate a log has the error log level 
 * `YAECPPL_LEVEL_FATAL` - used as an argument to indicate a log has the fatal error log level 
* Targets 
 * `YAECPPL_TARGET_STDOUT` - used within the target specification variables to indicate stdout as a target
 * `YAECPPL_TARGET_STDERR` - used within the target specification variables to indicate stderr as a target
 * `YAECPPL_TARGET_SQUELCH` - used within the target specification variables to indicate squelch as a target
 * `YAECPPL_TARGET_FILE` - used within the target specification variables to indicate the log file` as a target

## Function Overview 
### Public 
#### logger::logger(std::string targetFileName)
##### Arguments 
* `targetFileName` - the name of the log file to be opened. Still required even if `YAECPPL_TARGET_FILE` is not used. 

##### Returns 
Constructors have no return type. 

##### Use 
Initializes key variables within the logger class instance. 


#### int logger::verify()
##### Arguments 

##### Returns 
`YAECPPL_STATUS_SUCCESS` or `YAECPPL_STATUS_FAILURE` indicating success or failure. 

##### Use 
Checks all public variables of the logger instance to verify they have valid values. If all public variables are valid, returns `YAECPPL_STATUS_SUCCESS`, otherwise returns `YAECPPL_STATUS_FAILURE`. If `YAECPPL_VERIFY_PEDANTIC`, any function that calls `verify()` will immediately return `YAECPPL_STATUS_FAILURE` if `verify()` fails. 


#### int logger::logData(std::string name, data)
##### Arguments 
* `name` - a variable name or key
* `data` - the data to be logged. `logData()` is overloaded, so `data` may be any of: int, float, double, bool, or std::string

##### Returns 
`YAECPPL_STATUS_SUCCESS` or `YAECPPL_STATUS_FAILURE` indicating success or failure. 

##### Use 
Used to log data, such as the output of batch jobs, calculations, simulation results, etc. Generates a log message with the log level `YAECPPL_LEVEL_DATA`, and the contents `name=data`, naturally replacing `name` and `data` with the values of the corresponding variables. 


#### int logger::logMessage(std::string message, int logLevel)
##### Arguments 
* `message` - the message to be logged 
* `logLevel` - the log level of the message (optional, defaults to `YAECPPL_LEVEL_INFO`)

##### Returns 
`YAECPPL_STATUS_SUCCESS` or `YAECPPL_STATUS_FAILURE` indicating success or failure. 

##### Use 
Useful for quickly logging string messages. Generates a log entry with the specified message and log level. 


#### int logger::commit(int logLevel)
##### Arguments 
* `logLevel` - the log level of the message (optional, defaults to `YAECPPL_LEVEL_INFO`)

##### Returns 
`YAECPPL_STATUS_SUCCESS` or `YAECPPL_STATUS_FAILURE` indicating success or failure. 

##### Use 
When called, generates a log message with the specified log level. The contents of the message are the current contents of `logstream`, which are erased after `commit()` is called. 



### Private
#### std::string logger::generateTimestamp()
##### Arguments  

##### Returns 
String containing the current time stamp. 

##### Use 
Generates a time stamp in YYY:MM:DD:HH::mm:ss format in local time. Returns this as a string. 


#### bool logger::verifyVector(std::vector<int>& vectorToVerify, std::vector<int>& validValues)
##### Arguments 
* `vectorToVerify` - the vector to be verified 
* `validValues` - values which are valid for the indices of `vectorToVerify`  

##### Returns 
Bool, true if each index in `vectorToVerify` matches one of the indices of `validValues`, otherwise false. 

##### Use 
Used by `verify()` to check if a given set of targets are all valid. 


#### bool logger::vectorContains(std::vector<int>& vectorToCheck, int targetValue)
##### Arguments 
* `vectorToCheck` - the vector to be searched for `targetValue`
* `targetValue` - the value to search `vectorToCheck` for  

##### Returns 
Bool, true if any index of `vectorToCheck` contains `targetValue`, otherwise returns false. 

##### Use 
Used to check if `vectorToCheck` contains `targetValue` at least once. Designed to mimic the feature of python that allows statments like: 

```python
if foo in bar 
	print("bar contains ", foo)
```


#### int logger::logToTarget(std::string message, int target)
##### Arguments 
* `message` - the message to be logged 
* `target` - the target of the message, should be one of the constants listed in `Configuration/Other constants/Targets` 

##### Returns 
`YAECPPL_STATUS_SUCCESS` or `YAECPPL_STATUS_FAILURE` indicating success or failure. 

##### Use 
Used by `commit()` to dispatch log messages to the correct targets. Generates a log message with the contents `message` on the output specified in `target` 

