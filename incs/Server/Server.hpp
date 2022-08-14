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
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "NumericReplies.hpp"
#include "Channel.hpp"
#include "../../incs/ft_irc.h"
#include "NumericReplies.hpp"
#include "../../incs/User/User.hpp"

/***************** We begins the cmd enums here **************/
enum cmd {
	CAP			= 0,
	PASS		= 1,
	NICK		= 2,
	USER		= 3,
	JOIN		= 4,
	TOPIC		= 5,
	QUIT		= 6,
	MOTD		= 7,
	AWAY		= 8,
	VERSION		= 9,
	LUSERS 		= 10,
	PART        = 11,
	NAMES		= 12,
	LIST		= 13,
	INVITE		= 14,
	KICK		= 15,
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
		void	init();
		void	run();

/***************** Channel server part **************/
		void						addChannel(Channel * chan);
		bool						channelExist(std::string chanName);
		void						createChannel(int fd, User &user, Command &command);
		void						getAllChan(std::string chanName);
		void						printAllChannels();
		int							getnumberofchan();
		Channel						&getChannel(std::string);
		void						checkChannel(char *buffer, int fd);
		void 						printWelcomeMessage(int fd, User &user, Command &command, Channel *chan);
		void 						printAllChannelsUsers(User &user);
		void 						printListChannels(User &user);
		void 						displayListMinUser(User &user, int i);

/***************** Other parts **************/
		void						handleClientRequest(int i);
		void						handleCmd(User *user);
		void						sendToAll(int senderFd, std::string msg);
		std::vector<std::string>	getAdmin();
		int							getNbOfUsers();
		bool						userExist(std::string nickName);
		User						*getUser(int userFd);
		std::vector<std::string>	&getCmdList();
		User 						&nickToUserFd(std::string nickname);



		static void					registration(User *user);
		static void					welcome(User &user);
		static void					cmdDispatcher(Command &cmd, User &user);
};

#endif
