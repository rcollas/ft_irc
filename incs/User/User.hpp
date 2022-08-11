#ifndef FT_IRC_USER_HPP
#define FT_IRC_USER_HPP

#include <iostream>
#include "../../incs/ft_irc.h"
#include "../../incs/Server/Server.hpp"
#include "Server/Server.hpp"

/*
 * Check if registration is complete
 * Send a PING and ask for a PONG
 * if registration is complete server sends RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO and RPL_ISUPPORT
 * ex: std::string RPL_WELCOME(std::string prefix) { return "Welcome to the server" + prefix; }
 * everytime a client use a cmd, the server must send specific message
 * ex: JOIN cmd, a join message, RPL_TOPIC, RPL_NAMEPLY...
 */

class Server;
struct Command;
class Channel;

class User {

	private:
		std::string					nick_name;
		std::string					user_name;
		std::string					real_name;
		int							fd;
		std::vector<Command>		cmds;
		struct sockaddr_storage		socket;
		std::vector<Channel *>		activeChan;
		bool						admin;
		bool						away;

	public:
		User(std::vector<pollfd> &pfds, int serverEndpoint, Server *serverInfo);
		~User();

		Server					*servInfo;

		void	fill_information();
		int		get_fd() const;
		void	set_username(std::string username);
		void	set_nickname(std::string nickname);
		std::string	getNickName() const;
		std::string	getUserName() const;
		bool	isAdmin() const;

		void	addCmd(Command &cmd);
		void	addChan(Channel *chan);
		std::vector<Command>	&getCmdList();
};

std::ostream &operator<<(std::ostream &out, User &user);


#endif
