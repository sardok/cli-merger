INFO
====

Takes input file(s), sorts the parsed packages & merges into one final file while applying rate limiter to the merger in parallel.

The project generates 3 binaries.
* assignment - runs the main project
* tests - runs the tests
* gen_input - to generate 5 input files filled with random data.

Program parameters:

assignment
==========

-i <filename>   : specify input file (required, can be provided multiple times)
-o <filename>   : specify out file (default: out_file.txt)
-r <number>     : specify request rate (default: 100 per file)
-t <number>: time in seconds for request rate (-r 10 -t 5 means, max 10 request in 5 secs. default: 1 sec.)
-h              : prints help message.


Example command:
./assignment -i input_file0.txt -i input_file1.txt -i input_file2.txt -i input_file3.txt -i input_file4.txt -r 10000 -t 1

tests
=====
* Takes no argument

gen_input
=========
* Takes no argument. When executed the generated files will be named as "input_file0.txt", "input_file1.txt" so on...

DESIGN
======

There 4 major components:

* Packet:
It is the data of the program. Once parsed this data structure is carried between the components.
Supports the arithmetic and streaming operations.

* Parser:
The Parser is responsible for opening and parsing the input file. 
Its "parse" method generates one "Packet" at a time until the exhaustion of the file.

* MessageProcessor:
The MessageProcessor is a priority queue. It takes an item and saves it in a proper positions via the "push"
method and dumps all of its data in order via "flush" methods.
It is not thread-safe, so it must be executed by the rate limiter.

Note: The duplicates are not dropped as they are pushed but ignored during the flushed. This is a design 
choice to keep the priority queue's implementation simple.

* RateLimiter:
The rate limiter approach used here is called "Sliding Window Log".
I choose it because it is simple to implement.
The disadvantage of it is the space it takes. It keeps a logbook of the "process" in a queue.
So this logbook can grow as much as the request rate.
This class is thread-safe and should be used as a frontend to the MessageProcessor.

The multi-threading of the program is accomplished by calling a "std::async" for each input file.