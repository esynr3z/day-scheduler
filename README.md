# Simple C library to store some data in a day schedule

This library allows you to assign a dataset (integer, structure, function pointer) to any minute in a day.
You pass the current hour and minute through the API and get appropriate data. This can be useful in embedded systems in combination with RTC, e.g. switch relays, controlling LEDs and other tasks.

* no platform-depended code
* no dynamic memory allocaton
* schedule as a linked list
* entries can be added/removed in a runtime
