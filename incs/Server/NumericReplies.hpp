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

#define JOIN_WELCOME_MESSAGE(nick, channel) ("\e[0;34m" + str(nick) + " has joined " + str(channel) + "\r\n" + "\033[0m")
#define PART_LEAVE_CHANNEL_MESSAGE(nick, channel) ("\e[0;34m" + str(nick) + " has leaved " + str(channel) + "\r\n" + "\033[0m")

/******************************************************************************/
/*                                   0**                                      */
/******************************************************************************/
#define RPL_WELCOME(host, nick, user) ("001 " + str(nick) + " :Welcome to the " + str(host) + " network, " + str(nick) + "[!" + str(user) + "@" + str(host) + "]\r\n")


/******************************************************************************/
/*                                   2**                                      */
/******************************************************************************/
#define RPL_LUSERCLIENT(nick, users) ("\033[0;31m 251 " + str(nick) + " :There are " + str(users) + " user(s) on localhost\r\n\033[0m")
#define RPL_LUSEROP(nick, ops) ("\033[0;31m 252 " + str(nick) + " " + str(ops) + " :operator(s) online\r\n\033[0m")
#define RPL_LUSERCHANNELS(nick, channels) ("\033[0;31m 254 " + str(nick) + " " + str(channels) + " :channels formed\r\n\033[0m")
#define RPL_LUSERME(nick, clients) ("\033[0;31m 255 " + str(nick) + " :I have " + str(clients) " clients connected\r\n\033[0m")

/******************************************************************************/
/*                                   3**                                      */
/******************************************************************************/

#define RPL_TOPIC(nick, channel, topic) ("\e[0;34m 331 " + str(nick) + str(channel) + " :" + str(topic) + "\r\n" + "\033[0m")

#define RPL_NOTOPIC(channel) ("\e[0;34m 332: No topic set for " + str(channel) + "\r\n" + "\033[0m")

#define RPL_AWAY(nick, message) ("\033[0;31m 301 " + str(nick) + " :" + str(message) + "\r\n\033[0m")
#define RPL_UNAWAY(nick) ("\033[0;31m 305 " + str(nick) + " :You are no longer marked as being away\r\n\033[0m")
#define RPL_NOWAWAY(nick) ("\033[0;31m 306 " + str(nick) + " :You have been marked as being away\r\n\033[0m")
#define RPL_VERSION(nick) ("\033[0;31m 351 " + str(nick) = ": irssi 1.4.1 (20220612 1401)\r\n\033[0m")
#define RPL_NAMREPLY(nick) ("\e[0;34m 353 " + str(nick) + "\r\n" + "\033[0m")
#define RPL_MOTD(nick, text) ("\033[0;31m 372 " + str(nick) + " :- " + str(text) + "\r\n\033[0m")
#define RPL_MOTDSTART(nick, server) ("\033[0;31m 375 " + str(nick) + " :- " + str(server) + " Message of the day -\r\n\033[0m")
#define RPL_ENDOFMOTD(nick) ("\033[0;31m 376 " + str(nick) + " :End of /motd command\r\n\033[0m")


/******************************************************************************/
/*                                   4**                                      */
/******************************************************************************/

#define ERR_NOSUCHCHANNEL(nick, channel) ("\033[0;31m 403 " + str(nick) + " :" + str(channel) + " no such channel" + "\r\n" + "\033[0m")

#define ERR_NOTOCHANNEL(nick, channel) ("\033[0;31m 442 " + str(nick) + " :" + str(channel) + ": You're not on that channel" + "\r\n"+ "\033[0m")

#define ERR_NEEDMOREPARAMS(nick) ("\033[0;31m 461 " + str(nick) + " :Not enough parameters" +  "\r\n" + "\033[0m")

#define ERR_NONICKNAMEGIVEN() ("\033[0;31m 431 :No nickname given\r\n\033[0m")
#define ERR_ERRONEUSNICKNAME(nick) ("\033[0;31m 432 " + str(nick) + " :Erroneus nickname\r\n\033[0m")
#define ERR_NICKNAMEINUSE(nick) ("\033[0;31m 433 " + str(nick) + " :Nickname is already in use\r\n\033[0m")
#define ERR_ALREADYREGISTERED(nick)  ("\033[0;31m 462 " + str(nick) + " :You may not reregister\r\n\033[0m")


#endif
