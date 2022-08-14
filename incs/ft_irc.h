#ifndef INCS_FT_IRC
#define INCS_FT_IRC

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
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cerrno>
#include <vector>
#include <map>
#include "Server/Server.hpp"
#include "User/User.hpp"
#include "Server/Channel.hpp"

#define RECEIVE_DEBUG 1

class User;

struct Command {
	int							cmd;
	std::vector<std::string>	params;

	void	cap(Command &cmd, User &user);
	void	pass(Command &cmd, User &user);
	void	nick(Command &cmd, User &user);
	void	user(Command &cmd, User &user);
	void	join(Command &cmd, User &user);
	void	topic(Command &cmd, User &user);
	void	motd(Command &cmd, User &user);
	void	away(Command &cmd, User &user);
	void	version(Command &cmd, User &user);
	void	lusers(Command &cmd, User &user);
	void	part(Command &cmd, User &user);
	void	names(Command &cmd, User &user);
};

char	*ft_itoa(int nb);
void	printDebug(std::string msg, bool print);
std::vector<std::string>	split(std::string str, std::string const sep);
Command *parse(std::vector<std::string> &input, std::vector<std::string> cmdList);
void	printCmd(Command &cmdList);

#endif
