#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP


#include "../../incs/ft_irc.h"
#include "NumericReplies.hpp"
#include "../../incs/User/User.hpp"

enum cmd {
	CAP			= 0,
	PASS		= 1,
	NICK		= 2,
	USER		= 3,
};

class User;
struct Command;

class Server {

	private:
		int							serverEndPoint;
		socklen_t 					socketSize;
		std::string					portNum;
		static const int			bufferSize = 4096;
		static char					buffer[bufferSize];
		struct addrinfo				*serverinfo;
		struct sockaddr_in			*serverAddress;
		std::vector<pollfd>			pfds;
		std::string 				hostname;
		std::map<std::string, User>	user_list;
		std::vector<std::string>	cmdList;

		void	fillAvailableCmd();

	public:
		Server();
		~Server();

		void			init();
		void			run();
		std::vector<std::string>	&getCmdList();
		static void		registration(User &user);
		static void		welcome(int fd, std::string client_ip);
		static void		cmdDispatcher(Command &cmd, User &user);
};


#endif
