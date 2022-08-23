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
#include "../../incs/User/User.hpp"

/***************** We begins the cmd enums here **************/
enum cmd {
	PASS,
	NICK,
	USER,
	JOIN,
	TOPIC,
	QUIT,
	MOTD,
	AWAY,
	VERSION,
	LUSERS,
	PART,
	NAMES,
	LIST,
	INVITE,
	KICK,
	PRIVMSG,
	MODE,
	OPER,
	NOTICE,
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
		std::string							password;
		struct addrinfo						*serverinfo;
		struct sockaddr_in					*serverAddress;
		std::vector<pollfd>					pfds;
		std::string 						hostname;
		std::map<int, User>					user_list;
		std::vector<std::string>			cmdList;
		std::vector<std::string>			invisibleList;
		std::map<std::string, Channel *>	allChan;
		int 								nbOfOperators;
		std::string 						serverPassword;

		void	fillAvailableCmd();

	public:
		Server(std::string const port, std::string const passwd);
		~Server();
		void	init();
		void	run();

/***************** Channel server part **************/
		void						addChannel(Channel * chan);
		bool						channelExist(std::string chanName);
		bool						nicknameExists(std::string nickname);
		bool						usernameExists(std::string username);
		int							getTargetFd(std::string nickname);
		bool						getAwayStatus(std::string nickname);
		std::string 				getAwayString(std::string nickname);
		int							getModes(std::string nickname);
		void						createChannel(User &user, std::string chanName);
		void						getAllChan(std::string chanName);
		void						printAllChannels();
		int							getNbOfChan();
		Channel						&getChannel(std::string);
		void						checkChannel(char *buffer, int fd);
		void 						printWelcomeMessage(int fd, User &user, Command &command, Channel *chan);
		void 						printAllChannelsUsers(User &user);
		void 						printListChannels(User &user);
		void 						displayListMinUser(User &user, int i);
		void						deleteAllChannelUsers(User &user);


/***************** Other parts **************/
		void						handleClientRequest(int i);
		void						handleCmd(User *user);
		void						sendToAll(int senderFd, std::string msg);
		std::vector<std::string>	getAdmin();
		int							getNbOfUsers();
		bool						userExist(std::string nickName);
		User						*getUser(int userFd);
		std::vector<std::string>	&getCmdList();
		std::vector<std::string>	&getInvisibleList();
		User						&nickToUserFd(std::string nickname);
		void						killConnection(User &user);



		static void					registration(User *user);
		static void					welcome(User &user);
		static void					cmdDispatcher(Command &cmd, User &user);
		void						checkArgs(int ac, char **av);
		void						setPortNum(std::string portNum);
		std::string					getPassword() const;
		std::string					getServerPassword();
		int							get_isOperatorStatus();
};

#endif
