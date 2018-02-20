/*************************************************************************************************
* Author: Tida Sooreechine
* Date: 5/7/2017
* Program: CS372 Program 1, Part 2
* Description: A simple chat system client
*************************************************************************************************/

//reference:
//OSU CS344 Block 4 Lecture 2, "Network Clients" - by Professor Brewster

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>


//define global variables
char username[50];


//function prototypes
int createSocket(char* hostname, char* portNumber);
int receiveMessage(int socket);
int sendMessage(int socket);
void getUserHandle();


//main function
int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsSent;
	 
	//check usage and number of arguments
	if (argc != 3) {
		printf("Usage: ./chatclient [host] [port]\n");
		exit(0);
	}

	//get user's chat handle
	getUserHandle();

	//create a network socket and connect to server
	socketFD = createSocket(argv[1], argv[2]);
	charsSent = send(socketFD, username, strlen(username), 0);	//sending client username to server

	//send and receive messages from server
	while(1) {
		if(sendMessage(socketFD) < 0) {
			printf("Quitting chat session. Good bye.\n");
			break;
		} 
		
		if(receiveMessage(socketFD) < 0) {
			printf("Server has quit chat session. Logging out. Good bye.\n");
			break; 	
		}
	}

	close(socketFD);	//close client socket	
	return 0;
}


//function definitions

void getUserHandle() {
	int oneWord = 0;
	int lessThan10 = 0;
	int i;	

	//prompt user for username
	memset(username, '\0', sizeof(username));	//clear name buffer
	printf("Enter your username: ");			
	fgets(username, 50, stdin);

	//validate username - repeat loop indefinitely until a valid choice is given
	while((!lessThan10) || (!oneWord)) {
		//verify that the username is within characters limit
		if (((strlen(username) - 1) < 1) || ((strlen(username) - 1) > 10)) {
			lessThan10 = 0;
			printf("Invalid input. Username must be one-word and less than 10 characters.\n");
			printf("Enter your username: ");
			memset(username, '\0', sizeof(username));	//clear name buffer
			fgets(username, 50, stdin);
		} 
		else 
			lessThan10 = 1;

		//verify that the username is one-word
		oneWord = 1;
		for (i = 0; i < strlen(username); i++) {
			if(username[i] == ' ') {
				oneWord = 0;
				break;
			}
		}
		if (!oneWord) {
			printf("Invalid input. Username must be one-word and less than 10 characters.\n");
			printf("Enter your username: ");
			memset(username, '\0', sizeof(username));	//clear name buffer
			fgets(username, 50, stdin);
		}
	}
	
	//remove trailing new line character, if any
	strtok(username, "\n");
}

int createSocket(char* hostname, char* portNumber) {
	int sfd;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	//set up server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));	//clear out the address struct
	serverAddress.sin_family = AF_INET;	//create a network-capable socket
	serverAddress.sin_port = htons(atoi(portNumber));	//store the port number
	serverHostInfo = gethostbyname(hostname);	//convert machine name into appropriate address
	if (serverHostInfo == NULL) {
		printf("ERROR: Host does not exist.\n");
		exit(1);
	}	
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);	//copy in the address

	//create a TCP socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		printf("ERROR: Cannot create socket.\n");
		exit(1);
	}

	//connect to server
	if (connect(sfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		printf("ERROR: Cannot connect to server.\n");
	}
	else 
		printf("You are now connected to server. Chat session begins.\n");
	
	return sfd;
}

int sendMessage(int socket) {
	char userInput[501], buffer[600];
	int status = 1;
	int charsSent;
	
	//clear buffers
	memset(userInput, '\0', sizeof(userInput));
	memset(buffer, '\0', sizeof(buffer));

	//prompt user for input
	printf("%s> ", username);

	//store user input in buffer and remove trailing line
	fgets(userInput, 501, stdin);
	userInput[strcspn(userInput, "\n")] = '\0';

	//if user chooses to quit, set the global flag so that connection can be closed	
	//else proceed to send message to server
	if (strcmp(userInput, "\\quit") == 0) 
		status = -1; 
	
	strcat(buffer, username);
	strcat(buffer, "> ");
	strcat(buffer, userInput); 
	charsSent = send(socket, buffer, strlen(buffer), 0);	//write to server
	if (charsSent < 0) {
		printf("ERROR: Cannot write to socket.\n");
		exit(1);
	}

	return status;	
}

int receiveMessage(int socket) {
	char serverMessage[600];
	int status = 1;
	int charsReceived;
	
	//clear buffer
	memset(serverMessage, '\0', sizeof(serverMessage));

	//get response from server
	charsReceived = recv(socket, serverMessage, sizeof(serverMessage) - 1, 0);	//read data from the socket
	if (charsReceived < 0) {
		printf("ERROR: Cannot read from socket.\n");
		exit(1);
	}
	else 
		printf("%s\n", serverMessage);	//if no error reading data, print server's response to screen

	//determine if server has quit the chat session
	if (strstr(serverMessage, "\\quit") != NULL)
		status = -1;

	return status;	
}
