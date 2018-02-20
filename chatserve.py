#!/usr/bin/env python3

#-------------------------------------------------------------------------------------------------
# Author: Tida Sooreechine
# Date: 5/6/2017
# Program: CS372 Program 1, Part 1
# Description: A simple chat system server
#-------------------------------------------------------------------------------------------------

#reference(s) - note: other references are listed throughout the program
#OSU CS344 Block 4 Lecture 3, "Network Servers" - by Professor Brewster
#https://docs.python.org/3.3/howto/sockets.html
#https://docs.python.org/2/howto/sockets.html
#http://www.binarytides.com/python-socket-programming-tutorial/


import socket		#import socket module
import sys			#import sys module


def createSocket(portNumber):
	#create a socket object using TCP connection
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
	print("Socket created . . .")

	#bind the socket to host and port
	HOST = ''		#socket is reachable by any address the machine has
	PORT = portNumber	#user-specified port number
	try:
		s.bind((HOST, PORT))
	except socket.error: 
		print("ERROR: Bind failed.")
		exit(1)
	print("Socket binding completed . . .")
	
	#enable the socket to listen for connection
	s.listen(1)		

	return s


def receiveMessage(s):
	#store and reformat client's data
	clientByte = s.recv(600)
	clientString = clientByte.decode("utf-8")
	return clientString


def sendMessage(s):
	#get server user's input
	serverInput = input("sooreect> ")
	while len(serverInput) > 500:
		print("ERROR: Input exceeded 500 characters limit")
		serverInput = input("sooreect> ")

	#send server's response to client
	serverInput = "sooreect> " + serverInput
	serverString = serverInput.encode("utf-8")
	s.send(serverString)

	return serverInput


def main(argv):
	#check usage and number of arguments
	#source: http://www.pythonforbeginners.com/system/python-sys-argv
	if len(sys.argv) != 2: 
		print("Usage: python3 chatserve.py [port]")
		exit(1)

	#create a TCP network socket 
to listen for connections	
	s1 = createSocket(int(sys.argv[1]))
	print("Chat server started on port " + sys.argv[1])

	while 1: 
		print("Waiting for connection . . .")

		#establish connection with client
		s2, clientAddress = s1.accept()	
		clientNameByte = s2.recv(50) 	#byte object form	
		clientName = clientNameByte.decode("utf-8")
		print("Connected with " + clientName)
		#source: http://stackoverflow.com/questions/37016946/remove-b-character-do-in-front-of-a-string-literal-in-python-3

		q = "\quit"		#keyword for quitting chat session

		#start receiving messages from client
		while 1: 
			#receive message from client
			clientMessage = receiveMessage(s2)
			print(clientMessage)		
			
			#determine if the client has quit the chatroom
			if q in clientMessage:
				print(clientName + " has left the chatroom. Chat session ending . . .")
				break

			#send message to client
			serverMessage = sendMessage(s2)

			#determine if the server user wants to quit the chatroom
			if q in serverMessage:
				print("Quitting chat session.")
				break	
	
		s2.close()	#close the exclusive connection

	s1.close() #close the listening socket


if __name__ == "__main__":
    main(sys.argv)

