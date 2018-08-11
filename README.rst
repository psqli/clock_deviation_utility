=======================
Clock deviation utility
=======================

:Date: 2018-08-10

Helpers to get the average deviation of a clock and fix it.

The deviation is always linear. Spurious drifts (e.g.
from network latency) should be handled gracefully.

- deviation_history.h: ring buffer that stores the last
  N deviations. N is defined by the user.

- more_than_half.c: stores the sum of all values from a
  deviation history and, if more than half of the values
  are out of a predefined range, returns the deviation
  average.

- smooth_sync.h: helpers to distribute the deviation
  correction over time.


How to use
==========

Application stores the deviations in deviation_history,
gets the output from more_than_half, which uses
deviation_history, and calls smooth_sync to calculate
the synchronization smooth::

	+-------------+
	| application |------.
	+-------------+---.  |
	        |         |  |
	+-------------+   |  |
	| smooth_sync |   |  |
	+-------------+   |  |
	                  |  |
	+--------------------|--+
	| more_than_half     |  |
	| +-------------------+ |
	| | deviation history | |
	| +-------------------+ |
	+-----------------------+

The deviation_history can be used alone (outside
more_than_half).
