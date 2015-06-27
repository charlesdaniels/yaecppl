# Yet Another Easy C Plus Plus Logger 

## What is it? 
YAECPPL (pronounced "yaspel") is designed to be an easy to use, robust, written in correct C++11, and have no dependencies. Despite the name, at the time I started YACPPL, I could not find any easy to use C++ loggers with the feature set I needed, therefore I decided to write one. 

## Features 
* no dependencies 

* just instantiate one class object, then let yaecppl do the rest 

* highly configurable 

* log to a file, stdout, or stderr 

* log error messages separately from information 

* change where messages are logged during runtime 

* easy-to-parse output 

* optional timestamps 

## Installation 
Clone the repository somewhere convenient, run `make` and include `yaecppl.hpp` in your program. Make sure to include `yaecppl.o` in your make file! 

## Basic Usage 
First, make sure you have `yaecppl.hpp` included. Add the following line to your header: 

```cpp
#include "yaecppl.hpp"
```

Make sure `yaecppl.hpp` and `yaecppl.o` are in the same directory as your project, or somewhere your compiler can find them! 

Next, instantiate a logger instance: 

```cpp
logger log("foobar.log");
```

You can replace "foobar.log" with a filename of your choice. Note that even if you are not saving anything to a log file, the specified file must be writable. 

Now you are ready to start logging! You can create a simple log message like this: 

```cpp
log.logstream << "hello world!";
log.commit(); 
```

This will give you output similar to this: 

```
2015:06:27:15:06:09|info|hello world
```

You can also use one of five log levels:

* info

* data

* warning

* error

* fatal error 

Which correspond in that order to the constants

* YAECPPL_LEVEL_INFO

* YAECPPL_LEVEL_DATA

* YAECPPL_LEVEL_WARNING

* YAECPPL_LEVEL_ERROR

* YAECPPL_LEVEL_FATAL 

You can set the log level of a message by passing it's constant to `commit()` like this: 

```cpp
log.logstream << "an error occurred!";
log.commit(YAECPPL_LEVEL_ERROR); 
```

Which will give you output like this: 

```
2015:06:27:15:09:54|error|an error occurred
```


## Further Documentation 
Yaecppl is capable of much more, including logging data for later parsing, and logging to different destinations (stderr or a file). Review the `doc` directory for more examples and information. 