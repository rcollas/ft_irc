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

/***************** NUMERICS ERRORS **************/

std::string ERR_NEEDMOREPARAMS();
std::string	ERR_NOTONCHANNEL(std::string channelName);

/***************** OTHER **************/

/***************** CHANNELS NUMERICS **************/
std::string	JOINWELCOMEMESSAGE(std::string channel);
std::string	RPL_TOPIC(std::string channelName, std::string topic);
std::string	RPL_NOTOPIC(std::string channelName);

#define str(param) std::string(param)

/******************************************************************************/
/*                                   0**                                      */
/******************************************************************************/
#define RPL_WELCOME(host, nick, user) ("001 " + str(nick) + " :Welcome to the " + str(host) + " network, " + str(nick) + "[!" + str(user) + "@" + str(host) + "]\r\n")


/******************************************************************************/
/*                                   2**                                      */
/******************************************************************************/



/******************************************************************************/
/*                                   3**                                      */
/******************************************************************************/

#define RPL_AWAY(nick, message) ("301 " + str(nick) + " :" + str(message) + "\r\n")
#define RPL_UNAWAY(nick) ("305 " + str(nick) + " :You are no longer marked as being away\r\n")
#define RPL_NOWAWAY(nick) ("306 " + str(nick) + " :You have been marked as being away\r\n")
#define RPL_MOTD(nick, text) ("372 " + str(nick) + " :- " + str(text) + "\r\n")
#define RPL_MOTDSTART(nick, server) ("375 " + str(nick) + " :- " + str(server) + " Message of the day -\r\n")
#define RPL_ENDOFMOTD(nick) ("376 " + str(nick) + " :End of /MOTD command\r\n")

/******************************************************************************/
/*                                   4**                                      */
/******************************************************************************/


#endif
