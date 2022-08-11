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
#define RPL_WELCOME(host, nick, user) ("001 " + str(nick) + " :Welcome to the " + str(host) + " network, " + str(nick) + "[!" + str(user) + "@" + str(host) + "]\r\n")


/******************************************************************************/
/*                                   2**                                      */
/******************************************************************************/
#define RPL_LUSERCLIENT(nick, users) ("251 " + str(nick) + " :There are " + str(users) + " user(s) on localhost\r\n")
#define RPL_LUSEROP(nick, ops) ("252 " + str(nick) + " " + str(ops) + " :operator(s) online\r\n")
#define RPL_LUSERCHANNELS(nick, channels) ("254 " + str(nick) + " " + str(channels) + " :channels formed\r\n")
#define RPL_LUSERME(nick, clients) ("255 " + str(nick) + " :I have " + str(clients) " clients connected\r\n")

/******************************************************************************/
/*                                   3**                                      */
/******************************************************************************/

#define RPL_TOPIC(nick, text) ("331 " + str(nick) + " : " + str(text) + "\r\n")

#define RPL_NOTOPIC(channel) ("332 No topic set for " + str(channel) + "\r\n")

#define RPL_AWAY(nick, message) ("301 " + str(nick) + " :" + str(message) + "\r\n")
#define RPL_UNAWAY(nick) ("305 " + str(nick) + " :You are no longer marked as being away\r\n")
#define RPL_NOWAWAY(nick) ("306 " + str(nick) + " :You have been marked as being away\r\n")
#define RPL_VERSION(nick) ("351 " + str(nick) = ": irssi 1.4.1 (20220612 1401)\r\n")
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
