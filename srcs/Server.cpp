/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vincent <vincent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 16:32:09 by vincent           #+#    #+#             */
/*   Updated: 2022/08/03 14:47:14 by vincent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include<string.h>
#include<sys/types.h> //used for thread
#include<sys/socket.h> // to use all the functions for the internet protocol
#include<netinet/in.h> // allow to connect to a socket and send message
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

using namespace std;

int main()
{
	int client, server;
	int portNum = 1500; //the port number to communicate
	bool isExit = false; //Only if we want to leave the program
	int bufsize = 1024;
	char  buffer[bufsize]; //va servir à stocker les donner envoyées
	(void)isExit;
	(void)buffer;

	// Structure for specifies internet addresses
	struct sockaddr_in server_addr;
	socklen_t size; // unsigned integral type of length of at least 32 bit

	/*****************
	init the socket to communicate between client and server:
		AF_INET is the address family you can use for your socket (only IPV4 addresses)
		SOCK_STREAM is the type of socket:
		Provides sequenced, reliable, bidirectional, connection-mode byte streams, and may provide a transmission mechanism for out-of-band data.
		0 is the protocol by default
	**************/
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0)
	{
		cout << "Can't initiate the socket." << std::endl;
		exit(1);
	}
	std::cout << "Server socket connection created..." << std::endl;

	/*****************
	give all the values to the struct serve_addr:
	- sin_family représente le type de famille
	- sin_port représent the port to contact in
	- sin_addr représente l'adresse de l'hôte in network byte order (big endian, most significant byte first)
		INADDR_ANY means we bind the socket to all local interface
	- htons  translates an unsigned short integer into network byte order.
	**************/
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(portNum);

	/*****************
	 * binding socket
	– int bind(int sockfd, const struct sockaddr *my_addr,
		socklen_t addrlen);
	bind va prendre le fd, un pointeur sur la structure, et la taille de l'adresse
	On a donc bind dans le fd la structure et on pourra recup la taille de l'adresse
	**************/

	if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		cout << "Error binding socket ..." << endl;
		exit(1);
	}
	size = sizeof(server_addr);
	cout << "looking for clients ..." << endl;

	/*****************
	 * Listen
	– int listen(int sockfd, int backlog);
		Backlog = number of pending connections to queue
	**************/

	listen(client, 1);

	/*****************
	 * Accept the client
	– int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	**************/

	server = accept(client, (struct sockaddr*)&server_addr, &size);
	if (server < 0)
	{
		cout << "Error on accepting ..." << endl;
		exit(1);
	}

	return (0);
}