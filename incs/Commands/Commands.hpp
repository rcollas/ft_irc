#ifndef FT_IRC_COMMANDS_HPP
#define FT_IRC_COMMANDS_HPP


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

class Commands {

	private:

	public:
		Commands() {};
		~Commands() {};
        std::string RPL_MOTDSTART(std::string server);

};


#endif