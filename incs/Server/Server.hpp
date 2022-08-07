#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <vector>
#include "NumericReplies.hpp"
#include "Channel.hpp"


class Server {

	private:
		int					serverEndPoint;
		socklen_t 			socketSize;
		std::string			portNum;
		static const int	bufferSize = 4096;
		char				buffer[bufferSize];
		struct addrinfo		*serverinfo;
		struct sockaddr_in	*serverAddress;
		std::vector<pollfd>	pfds;
		std::string 		hostname;

	public:
		Server();
		~Server();

		void		init();
		void		run();
		void		welcome(int fd, std::string client_ip);
		void		checkChannel(char *buffer, int fd); // à enlever après parsing Robin
};

#endif
