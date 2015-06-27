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