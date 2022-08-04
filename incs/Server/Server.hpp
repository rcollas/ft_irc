#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <cerrno>
#include <vector>


class Server {

	private:
		int					serverEndPoint;
		socklen_t 			socketSize;
		int					portNum;
		static const int	bufferSize = 1024;
		char				buffer[bufferSize];
		struct sockaddr_in	serverAddress;
		std::vector<pollfd>	pfds;

	public:
		Server();
		~Server();

		int		init();
		void	run();
		void	addUser(int i);
};


#endif
