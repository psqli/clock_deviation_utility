=======================
Clock deviation utility
=======================

:Date: 2018-08-10

Helpers to get the average deviation of a clock and fix it.

The deviation is always linear. Spurious drifts (e.g.
from network latency) should be handled gracefully.

- deviation_history.h: ring buffer that stores the last
  N deviations. N is defined by the user.

- deviation_average.c: stores the sum of all values from a
  deviation history and, if more than half of the values
  are out of a predefined range, returns the deviation
  average.

- smooth_correction.h: helpers to distribute the deviation
  correction over time.


How to use
==========

Application stores the deviations in deviation_history,
gets the output from deviation_average, which uses
deviation_history, and calls smooth_correction to calculate
the synchronization smooth::

	+-------------+
	| application |---------.
	+-------------+-------. |
	        |             | |
	+-------------------+ | |
	| smooth_correction | | |
	+-------------------+ | |
	                      | |
	+---------------------|---+
	| deviation_average   |   |
	| +--------------------+  |
	| | deviation_history  |  |
	| +--------------------+  |
	+-------------------------+

The deviation_history can be used alone (outside
deviation_average).
