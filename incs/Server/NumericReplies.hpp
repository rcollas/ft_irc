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
#define PART_LEAVE_CHANNEL_MESSAGE(nick, channel) ("\e[0;34m" + str(nick) + " has left " + str(channel) + "\r\n" + "\033[0m")
#define INVITE_WELCOME_MESSAGE(nick, channel) ("\e[0;34m" + str(nick) + " has invited you to " + str(channel) + "\r\n" + "\033[0m")
#define KICK__MESSAGE(nick, channel, reason) ("\e[0;34m" + str(nick) + " has kicked you from " + str(channel) + " for the reason :" + str(reason)+ "\r\n" + "\033[0m")

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

#define RPL_TOPIC(nick, channel, topic) ("\e[0;34m 331 " + str(nick) + " " + str(channel) + " :" + str(topic) + "\r\n" + "\033[0m")

#define RPL_NOTOPIC(channel) ("\e[0;34m 332: No topic set for " + str(channel) + "\r\n" + "\033[0m")

#define RPL_AWAY(nick, message) ("301 " + str(nick) + " :" + str(message) + "\r\n")
#define RPL_UNAWAY(nick) ("305 " + str(nick) + " :You are no longer marked as being away\r\n")
#define RPL_NOWAWAY(nick) ("306 " + str(nick) + " :You have been marked as being away\r\n")
#define RPL_LIST(channel, count, topic) ("\e[0;34m 322 " + str(channel) + " " + str(count) + " :" + str(topic) + "\r\n" + "\033[0m")
#define RPL_LISTEND(nick)("\e[0;34m 323 " + str(nick) + " :END of LIST" + "\r\n" + "\033[0m")
#define RPL_INVITING(nick, channel) ("\e[0;34m 341 inviting " + str(nick) + " to " + str(channel) + "\r\n" + "\033[0m")
#define RPL_VERSION(nick) ("351 " + str(nick) = ": irssi 1.4.1 (20220612 1401)\r\n")
#define RPL_NAMREPLY(nick, channel) ("\e[0;34m 353 "+ str(channel) + " :" + str(nick) + "\r\n" + "\033[0m")
#define RPL_ENDOFNAMES(channel) ("\e[0;34m 366 " + str(channel) + " :End of NAMES list" + "\r\n" + "\033[0m")
#define RPL_MOTD(nick, text) ("372 " + str(nick) + " :- " + str(text) + "\r\n")
#define RPL_MOTDSTART(nick, server) ("375 " + str(nick) + " :- " + str(server) + " Message of the day -\r\n")
#define RPL_ENDOFMOTD(nick) ("376 " + str(nick) + " :End of /MOTD command\r\n")


/******************************************************************************/
/*                                   4**                                      */
/******************************************************************************/

#define ERR_NOSUCHCHANNEL(nick, channel) ("\033[0;31m 403 " + str(nick) + " :" + str(channel) + " no such channel" + "\r\n" + "\033[0m")
#define ERR_NOTOCHANNEL(nick, channel) ("\033[0;31m 442 " + str(nick) + " :" + str(channel) + ": You're not on that channel" + "\r\n"+ "\033[0m")
#define ERR_NEEDMOREPARAMS(nick) ("\033[0;31m 461 " + str(nick) + " :Not enough parameters" +  "\r\n" + "\033[0m")
#define ERR_NONICKNAMEGIVEN() ("431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME() ("\033[0;31m 432 :Erroneus nickname\r\n \033[0m")
#define ERR_NICKNAMEINUSE(nick) ("433 " + str(nick) + " :Nickname is already in use\r\n")
#define ERR_USERONCHANNEL(nick, channel) ("\033[0;31m 443 " + str(nick) + " " + str(channel) + " :is already on channel" + "\r\n" + "\033[0m")
#define ERR_USERNOTINCHANNEL(nick, channel) ("\033[0;31m 443 " + str(nick) + " " + str(channel) + " :isn't already on that channel" + "\r\n" + "\033[0m")


#endif
