#ifndef FT_IRC_NUMERICREPLIES_HPP
#define FT_IRC_NUMERICREPLIES_HPP

#include <iostream>
#include "User/User.hpp"

class User;

void	sendMsg(int fd, std::string msg);
std::string RPL_YOURHOST();
std::string	RPL_CREATED();
std::string	RPL_MYINFO();
std::string RPL_ISUPPORT();

#define str(param) std::string(param)

#define RPL_WELCOME(host, nick, user) ("001 " + str(nick) + " :Welcome to the " \
										+ str(host) + " network, " + str(nick) \
										+ "[!" + str(user) + "@" + str(host) + "]\r\n")




#endif
