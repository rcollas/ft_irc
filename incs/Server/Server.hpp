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
	JOIN		= 4,
};

class User;
struct Command;

class Server {

	private:
		int									serverEndPoint;
		socklen_t 							socketSize;
		std::string							portNum;
		static const int					bufferSize = 4096;
		static char							buffer[bufferSize];
		struct addrinfo						*serverinfo;
		struct sockaddr_in					*serverAddress;
		std::vector<pollfd>					pfds;
		std::string 						hostname;
		std::map<int, User>					user_list;
		std::vector<std::string>			cmdList;
		std::map<std::string, Channel *>	allChan;

		void	fillAvailableCmd();

	public:
		Server();
		~Server();

		void			init();
		void			run();

		void			handleClientRequest(int i);
		void			sendToAll(int senderFd, int nbytes);

		void			addChannel(Channel * chan);

		std::vector<std::string>	&getCmdList();

		static void		registration(User &user);
		static void		welcome(User &user);
		static void		cmdDispatcher(Command &cmd, User &user);
};


#endif
