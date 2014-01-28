# phpTimer - High Precision Timer Extension

`phpTimer` provides a nanosecond-precision timer to PHP.

##Usage

	$start = timerStart();

	// Do a bunch of things here that take some time.
	for ($i = 0; $i < 10000; $i++) {}

	$checkpoint = timerCheckpoint();

	// Do some more time-vampire things here.
	for ($i = 0; $i < 10000; $i++) {}

	$stop = timerStop();

	echo timerDiff($start, $stop) . "\n"; // Total execution time.
	echo timerDiff($start, $checkpoint) . "\n"; // Execution time of first half
	echo timerDiff($checkpoint, $stop) . "\n"; // Execution time of second half

	echo timerGet() . "\n"; // Time since cTimer was imported

## Methods

**`timerStart()`** - returns an `int` referencing the current time stamp.

**`timerCheckpoint()`** - returns an `int` referencing the current time stamp.

**`timerStop()`** - returns an `int` referencing the current time stamp.

**`timerDiff(int A, int B)`** - returns the difference between two timestamp references provided by `timerStart()`, `timerCheckpoint()` or `timerStop()`. Order does not matter, result is always a positive `float` representing seconds.

**`timerGet()`** - returns the number of seconds as a `float` since the PHP process was started.

## Installation

Clone the repository into a new folder. Run the following commands (you *may* need to `sudo` the third one):

	phpize
	./configure
	make install

## Notes

This is a lightweight extension and has not been thouroughly tested on all combinations of platforms and PHP versions.

It has been tested on CentOS 5 and 6, Ubuntu 10.x to 13.x, and OSX 10.5 to 10.9 using PHP 5.4. All platforms were 64-bit.

This *should* work with 32-bit hosts, but that has not been tested yet.

This *does not* work on Windows hosts yet.