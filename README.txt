Author: Tida Sooreechine
Date: 5/7/2017
Program: CS372 Program 1
Description: A simple chat system, consisting of a server program, written in Python3, and a 
	client program, written in C language
--------------------------------------------------------------------------------------------------
Steps to get the chat system started:

1.	To get the chat server running, run the program with the following command:
		python3 chatserver.py [port]
	where
		[port] is a number from 0-65000 (approx.). It is recommended that a number larger than 
			10000 is used, in order to avoid using well-known ports, associated with other 
			applications.

2. 	After the chat server has successfully started running, on a separate terminal window, compile
	the client program with the following command:
		make
	This will activate the makefile, which will complete the compiling process for you.

3.	Execute the client program with the following command:
		./chatclient [host] [port]
	where
		[host] is flip1, flip2, or flip3
		[port] is the same port number, used to run the chat server process in step 1. 

4. 	Follow the user prompt on the client process to initiate chat connection with the server.

5.	To quit the program, either type "\quit" into the prompt or press CTRL+Z.

