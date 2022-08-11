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
std::string RPL_ISUPPORT();

#define str(param) std::string(param)

#define JOIN_WELCOME_MESSAGE(nick, channel) (str(nick) + " has joined " + str(channel) + "\r\n")

/******************************************************************************/
/*                                   0**                                      */
/******************************************************************************/
#define RPL_WELCOME(host, nick, user) ("001 " + str(nick) + " :Welcome to the " \
										+ str(host) + " network, " + str(nick) \
										+ "[!" + str(user) + "@" + str(host) + "]\r\n")


/******************************************************************************/
/*                                   2**                                      */
/******************************************************************************/

/******************************************************************************/
/*                                   3**                                      */
/******************************************************************************/

#define RPL_TOPIC(nick, text) ("331 " + str(nick) + " : " + str(text) + "\r\n")

#define RPL_NOTOPIC(channel) ("332 No topic set for " + str(channel) + "\r\n")

#define RPL_MOTD(nick, text) ("372 " + str(nick) + " :- " + str(text) + "\r\n")

#define RPL_MOTDSTART(nick, server) ("375 " + str(nick) + " :- " + str(server) + " Message of the day -\r\n")

#define RPL_ENDOFMOTD(nick) ("376 " + str(nick) + " :End of /MOTD command\r\n")

/******************************************************************************/
/*                                   4**                                      */
/******************************************************************************/

#define ERR_NOSUCHCHANNEL(nick, channel) ("403 " + str(nick) + " " + str(channel) + " no such channel" + "\r\n")

#define ERR_NOTONCHANNEL(nick, channel) ("442 " + str(nick) + str(channel) + ": You're not on that channel" + "\r\n")

#define ERR_NEEDMOREPARAMS(nick) ("461 " + str(nick) + " :Not enough parameters" +  "\r\n")


#endif
