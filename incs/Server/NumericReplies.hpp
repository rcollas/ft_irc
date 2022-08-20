#ifndef FT_IRC_NUMERICREPLIES_HPP
#define FT_IRC_NUMERICREPLIES_HPP

#include <iostream>
#include "User/User.hpp"

#define str(param) std::string(param)

#define EOC str("\033[0m")
#define BLUE str("\033[0;34m")
#define GREEN str("\033[1;32m")
#define RED str("\033[0;31m")

class User;

void	sendMsg(int fd, std::string msg);
std::string RPL_YOURHOST();
std::string	RPL_CREATED();
std::string	RPL_MYINFO();
std::string RPL_ISUPPORT();
std::string RPL_ISUPPORT();


#define JOIN_WELCOME_MESSAGE(nick, channel) (BLUE + str(nick) + " has joined " + str(channel) + "\r\n" + EOC)
#define PART_LEAVE_CHANNEL_MESSAGE(nick, channel, message) (BLUE + str(nick) + " has left " + str(channel) + str(message) + "\r\n" + EOC)
#define INVITE_WELCOME_MESSAGE(nick, channel) (BLUE + str(nick) + " has invited you to " + str(channel) + "\r\n" + EOC)
#define KICK__MESSAGE(nick, channel, reason) (BLUE + str(nick) + " has kicked you from " + str(channel) + " for the reason :" + str(reason)+ "\r\n" + EOC)
#define PRV_MSG(nick, message) (GREEN + str(nick) + " :" + str(message) + "\r\n" + EOC)
/******************************************************************************/
/*                                   0**                                      */
/******************************************************************************/

#define RPL_WELCOME(host, nick, user) ("001 " + str(nick) + " :Welcome to the " + str(host) + " network, " + str(nick) + "[!" + str(user) + "@" + str(host) + "]\r\n")
#define RPL_YOURHOST(nick) ("002 " + str(nick) + " :Your host is 127.0.0.1, running version 1.2.3\r\n")
#define RPL_CREATED(nick) ("003 " + str(nick) + " :This server was created 18:07:30\r\n")
#define RPL_MYINFO(nick) ("004 " + str(nick) + " localhost irssi 1.4.1 (20220612 1401) 1 2\r\n")
#define RPL_ISUPPORT(nick) ("005 " + str(nick) + " 12 :are supported by this server\r\n")

/******************************************************************************/
/*                                   2**                                      */
/******************************************************************************/

#define RPL_UMODEIS(nick, modes) (RED + "251 " + str(nick) + " " + str(modes) + "\r\n" + EOC)
#define RPL_LUSERCLIENT(nick, users) (RED + "251 " + str(nick) + " :There are " + str(users) + " user(s) on localhost\r\n" + EOC)
#define RPL_LUSEROP(nick, ops) (RED + "252 " + str(nick) + " " + str(ops) + " :operator(s) online\r\n" + EOC)
#define RPL_LUSERCHANNELS(nick, channels) (RED + "254 " + str(nick) + " " + str(channels) + " :channels formed\r\n" + EOC)
#define RPL_LUSERME(nick, clients) (RED + "255 " + str(nick) + " :I have " + str(clients) " clients connected\r\n\033[0m")

/******************************************************************************/
/*                                   3**                                      */
/******************************************************************************/

#define RPL_AWAY(nick, message) (RED + "301 " + str(nick) + " :" + str(message) + "\r\n" + EOC)
#define RPL_UNAWAY(nick) (RED + "305 " + str(nick) + " :You are no longer marked as being away\r\n" + EOC)
#define RPL_NOWAWAY(nick) (RED + "306 " + str(nick) + " :You have been marked as being away\r\n" + EOC)
#define RPL_TOPIC(nick, channel, topic) (BLUE + "331 " + str(nick) + " " + str(channel) + " :" + str(topic) + "\r\n" + EOC)
#define RPL_NOTOPIC(channel) (BLUE + "332: No topic set for " + str(channel) + "\r\n" + EOC)
#define RPL_VERSION(nick) (RED + "351 " + str(nick) + " :irssi 1.4.1 (20220612 1401)\r\n" + EOC)
#define RPL_MOTD(nick, text) (RED + "372 " + str(nick) + " :- " + str(text) + " -\r\n" + EOC)
#define RPL_MOTDSTART(nick, server) (RED + "375 " + str(nick) + " :- " + str(server) + " Message of the day -\r\n" + EOC)
#define RPL_ENDOFMOTD(nick) (RED + "376 " + str(nick) + " :End of /motd command\r\n" + EOC)
#define RPL_LIST(channel, count, topic) (BLUE + "322 " + str(channel) + " " + str(count) + " :" + str(topic) + "\r\n" + EOC)
#define RPL_LISTEND(nick)(BLUE + "323 " + str(nick) + " :END of LIST" + "\r\n" + EOC)
#define RPL_INVITING(nick, channel) (BLUE + "341 inviting " + str(nick) + " to " + str(channel) + "\r\n" + EOC)
#define RPL_NAMREPLY(nick, channel) (BLUE + "353 "+ str(channel) + " :" + str(nick) + "\r\n" + EOC)
#define RPL_ENDOFNAMES(channel) (BLUE + "366 " + str(channel) + " :End of NAMES list" + "\r\n" + EOC)
#define RPL_YOUREOPER(nick)(BLUE + "381 " + str(nick) + " :You are now an IRC operator" + "\r\n" + EOC)

/******************************************************************************/
/*                                   4**                                      */
/******************************************************************************/

#define ERR_NOSUCHNICK(nick, target) (RED + "401 " + str(nick) + " " + str(target) + " :No such nick/channel\r\n" + EOC)
#define ERR_NOSUCHCHANNEL(nick, channel) (RED + "403 " + str(nick) + " :" + str(channel) + " no such channel" + "\r\n" + EOC)
#define ERR_NOTOCHANNEL(nick, channel) (RED + "442 " + str(nick) + " :" + str(channel) + ": You're not on that channel" + "\r\n"+ EOC)
#define ERR_NONICKNAMEGIVEN() (RED + "431 :No nickname given\r\n" + EOC)
#define ERR_ERRONEUSNICKNAME(nick) (RED + "432 " + str(nick) + " :Erroneus nickname\r\n" + EOC)
#define ERR_NICKNAMEINUSE(nick) (RED + "433 " + str(nick) + " :Nickname is already in use\r\n" + EOC)
#define ERR_USERONCHANNEL(nick, channel) (RED + "443 " + str(nick) + " " + str(channel) + " :is already on channel" + "\r\n" + EOC)
#define ERR_USERNOTINCHANNEL(nick, channel) (RED + "443 " + str(nick) + " " + str(channel) + " :isn't already on that channel" + "\r\n" + EOC)
#define ERR_NEEDMOREPARAMS(nick) (RED + "461 " + str(nick) + " :Not enough parameters" +  "\r\n" + EOC)
#define ERR_ALREADYREGISTERED(nick)  (RED + "462 " + str(nick) + " :You may not reregister\r\n" + EOC)
#define ERR_PASSWDMISMATCH(nick) (RED + "464 " + str(nick) + " :Password incorrect\r\n" + EOC)
#define ERR_INVITEONLYCHAN(nick, channel) (RED + "473 " + str(nick) + " " + str(channel) + " :cannot join the channel (+i)\r\n" + EOC)
#define ERR_BADCHANNELKEY(nick, channel)  (RED + "475 " + str(nick) + " " + str(channel) + " :cannot join the channel (+k)\r\n" + EOC)
#define ERR_NOPRIVILEGES(nick) (RED + "481 " + str(nick) + " :Permission Denied- You're not an IRC operator\r\n" + EOC)

/******************************************************************************/
/*                                   5**                                      */
/******************************************************************************/

#define ERR_UMODEUNKNOWNFLAG(nick) (RED + "501 " + str(nick) + " :Unknown MODE flag\r\n" + EOC)
#define ERR_USERSDONTMATCH(nick) (RED + "502 " + str(nick) + " :Cant change mode for other users\r\n" + EOC)
#define ERR_INVALIDKEY(nick, channel) (RED + "525 :" + str(nick) + " " + str(channel) + " :Key is not well-formed\r\n" + EOC)

/******************************************************************************/
/*                                   6**                                      */
/******************************************************************************/

#define ERR_INVALIDMODEPARAM (nick, channel, mode, parameter, description) (RED + "696 :" + str(nick) + " " + str(channel) + " " + str(mode) + " " + str(parameter) + " :" str(description) + "\r\n\033[0m")


#endif
