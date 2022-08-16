#include "../../incs/ft_irc.h"
#include "../../incs/Server/Server.hpp"


std::string	concatenate_strings(std::string first, std::string second)
{
	first = first.append(" ");
	first = first.append(second);
	return (first);
}

void	Command::cap(Command &command, User &user) {
	(void)command;
	send(user.get_fd(), "CAP\r\n", strlen("CAP\r\n"), 0);
}

void	Command::pass(Command &command, User &user) {
	if (command.params.size() > 1)
		return ;
	if (command.params.size() == 1 && command.params[0] != user.servInfo->getPassword())
		sendMsg(user.get_fd(), ERR_PASSWDMISMATCH(user.getNickName()));
}

void	Command::nick(Command &command, User &user) {
	if (command.params.size() > 1)
		return ;
	else if (command.params.empty() == true)
		sendMsg(user.get_fd(), ERR_NONICKNAMEGIVEN());
	else {
		if (user.getNickName() == command.params[0] || user.servInfo->nicknameExists(command.params[0]) == false)
			sendMsg(user.get_fd(), ERR_NICKNAMEINUSE(user.getNickName()));
		else if (command.params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_-") != std::string::npos)
			sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME(user.getNickName()));
		else {
			if (user.getNickName().empty() == true) {
				user.set_nickname(command.params[0]);
				std::cout << "Nickname is set at : " << command.params[0] << std::endl;
				return ;
			}
			else {
				user.servInfo->sendToAll(user.get_fd(), user.getNickName() + "!@localhost NICK " + command.params[0] + "\n");
				user.set_nickname(command.params[0]);
				std::cout << "Nickname is set at " << command.params[0] << std::endl;
			}
		}
	}
}

void	Command::user(Command &command, User &user) {
	if (command.params.size() > 4)
		return ;
	if (command.params.size() < 4) {
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
		return ;
	}
	if (!(command.params[1] == "0" && command.params[2] == "*")) {
		std::cout << "Usage: /USER <username> 0 * <realname>\n";
		return ;
	}
	if (command.params[0].length() < 1 || command.params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
		std::cout << "Username contains invalid characters\n";
	}
	if (command.params[3].length() < 1 || command.params[3].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
		std::cout << "Realname contains invalid characters\n";
		return ;
	}
	if (user.getUserName().empty() == false) {
		sendMsg(user.get_fd(), ERR_ALREADYREGISTERED(user.getNickName()));
		return ;
	}
	if (user.servInfo->usernameExists(command.params[0]) == false) 
		std::cout << "Username already exists on the server\n";
	else {
		user.set_username(command.params[0]);
		std::cout << "Username is set at : " << command.params[0] << std::endl;
		user.set_realname(command.params[3]);
		std::cout << "Realname is set at : " << command.params[3] << std::endl;
	}
}

void	Command::privmsg(Command &command, User &user) {
	if (command.params.size() != 2) {
		return ;
	}
	if (user.servInfo->usernameExists(command.params[0]) == false || user.servInfo->nicknameExists(command.params[0]) == false) {
		if (user.servInfo->getAwayStatus(command.params[0]) == true) {
			sendMsg(user.get_fd(), "\033[0;31m301 " + command.params[0] + " :" + user.servInfo->getAwayString(command.params[0]) + "\r\n\033[0m");
		}
		sendMsg(user.servInfo->getTargetFd(command.params[0]), "\033[1;32m" + command.params[1] + "\r\n\033[0m");
	}
}

void	Command::motd(Command &command, User &user) {
	if (command.params.empty() == true) {
		sendMsg(user.get_fd(), RPL_MOTDSTART(user.getNickName(), "localhost"));
		sendMsg(user.get_fd(), RPL_MOTD(user.getNickName(), "Welcooooooooooome at home!"));
		sendMsg(user.get_fd(), RPL_ENDOFMOTD(user.getNickName()));
	}
}


void	Command::mode(Command &command, User &user) {
		if (command.params.size() > 4)
			return ;
		if (user.servInfo->nicknameExists(command.params[0]) == true) {
			sendMsg(user.get_fd(), ERR_NOSUCHNICK(user.getNickName(), command.params[0]));
			return ;
		}
		if (command.params[0] != user.getNickName()) {
			sendMsg(user.get_fd(), ERR_USERSDONTMATCH(user.getNickName()));
			return ;
		}
		// if (command.params[0] == user.getNickName()) {
		// 	if (command.params.size() == 1) { // display user modes if no 2nd param
		// 		sendMsg(user.get_fd(), ("\033[0;31m221 " + user.getNickName() + " :" + user.getModesNumber() + "\r\n\033[0m"));
		// 	}
		// 	return ;
		// }
		if (!isAllowedMode(command.params[1])) {
			sendMsg(user.get_fd(), ERR_UMODEUNKNOWNFLAG(user.getNickName()));
		}
		if (isAllowedMode(command.params[1]) && command.params[0] == user.getNickName() && command.params[1] == "-i") {
			;
		}
}

void	Command::away(Command &command, User &user) {
	if (command.params.empty() == true && user.getIsAway() == true) {
		user.set_isAway(false);
		sendMsg(user.get_fd(), RPL_UNAWAY(user.getNickName()));
	}
	if (command.params.empty() == false) {
		user.set_isAway(true);
		user.set_awayMessage(command.params[0]);
		sendMsg(user.get_fd(), RPL_NOWAWAY(user.getNickName()));
		sendMsg(user.get_fd(), RPL_AWAY(user.getNickName(), command.params[0]));
	}
}

void	Command::version(Command &command, User &user) {
	if (command.params.empty() == true)
		sendMsg(user.get_fd(), RPL_VERSION(user.getNickName()));
}

void	Command::lusers(Command &command, User &user)
{
	if (command.params.empty() == true) {
		sendMsg(user.get_fd(), RPL_LUSERCLIENT(user.getNickName(), ft_itoa(user.servInfo->getNbOfUsers())));
		sendMsg(user.get_fd(), RPL_LUSEROP(user.getNickName(), "0"));
		sendMsg(user.get_fd(), RPL_LUSERCHANNELS(user.getNickName(), "getNumberOfChan"));
		//sendMsg(user.get_fd(), RPL_LUSERME(user.getNickName(), ft_itoa(user.servInfo->getNbOfUsers())));
	}
}

/*
**==========================
**    CHANNEL COMMAND
**==========================
*/

void	Command::join(Command &command, User &user) {
	if (command.params.empty() == false)
	{
		user.servInfo->createChannel(user.get_fd(), user, command);
		//user.servInfo->printAllChannels(); // to remove
		for (unsigned long i = 0 ; i < command.params.size() ; i++)
		{
			Channel *chan = &user.servInfo->getChannel(command.params[i]);
			chan->addUserToChannel(user.get_fd(), &user);
			//chan->printChannelUsers(user.get_fd(), &user); // to remove
			user.servInfo->printWelcomeMessage(user.get_fd(), user, command, chan);
			if (chan->TopicIsSet() == true)
				sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
			else
				sendMsg(user.get_fd(), RPL_NOTOPIC(chan->getChannelName()));
		}
	}
	else 
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
}

/***************** I set up the topic of the channel
 *  I set the topic here to send to the client 
 * I need to have at least 3 param TOPIC <channel> <message> **************/
void	SetTopic(Command &command, User &user)
{
	Channel *chan = &user.servInfo->getChannel(command.params[0]);
	if (command.params.size() >= 2 && chan->userInChannel(user.get_fd()) == true)
	{
		std::string topic;
		for (unsigned long i = 1; i <= command.params.size(); i++)
			topic = concatenate_strings(topic, command.params[i]);
		chan->changeTopic(topic);
		sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
	}
}


void	sendTopic(Command &command, User &user)
{
	Channel *chan = &user.servInfo->getChannel(command.params[0]);
	if (command.params.size() == 1 && chan->userInChannel(user.get_fd()) == true && chan->TopicIsSet() == true)
		sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
	else if (command.params.size() == 1 && chan->userInChannel(user.get_fd()) == true && chan->TopicIsSet() == false)
		sendMsg(user.get_fd(), RPL_NOTOPIC(chan->getChannelName()));
}

/***************** Topic allows
 * to handle a topic for a dedicated channel **************/
void	Command::topic(Command &command, User &user) {
	if (command.params.empty() == false)
	{
		if (user.servInfo->channelExist(command.params[0]) == true) //I verify if param[1](chanName exist)
		{
				SetTopic(command, user);
				sendTopic(command, user);
		}
		else if (user.servInfo->channelExist(command.params[0]) == false) // if the channel doesn't exist = error
			sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[0]));
	}
	else 
	{
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}

/***************** PART allows 
 * temove a user from one+ channels 
 * first condition : I check if channel exist and i am in the channel
 * second condition : I check if channel exist and i am not in the channel
 * third condition the channel doesn't exist **************/
void	Command::part(Command &command, User &user) {
	if (command.params.empty() == false)
	{
		for (unsigned long i = 0 ; i < command.params.size()  ; i++)
		{
			if (user.servInfo->channelExist(command.params[i]) == true) 
			{
				Channel *chan = &user.servInfo->getChannel(command.params[i]);
				if (chan->userInChannel(user.get_fd()) == true)
				{
					chan->printChannelUsers(user.get_fd(), &user, command.params[i]); // to remove
					chan->removeUserChannel(user.get_fd(), &user);
					chan->printChannelUsers(user.get_fd(), &user, command.params[i]); //to remove
				}
				else if (chan->userInChannel(user.get_fd()) == false)
					sendMsg(user.get_fd(), ERR_NOTOCHANNEL(user.getNickName(), chan->getChannelName()));

			}
			else 
				sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[i]));
		}
	}
	else 
	{
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}

/***************** NAMES allows
 * display the list of users of specified channels **************/
void	Command::names(Command &command, User &user) {
	if (command.params.empty() == false)
	{
		for (unsigned long i = 0 ; i < command.params.size() ; i++)
		{
			if (user.servInfo->channelExist(command.params[i]) == true) 
			{
				Channel *chan = &user.servInfo->getChannel(command.params[i]);
				chan->printChannelUsers(user.get_fd(), &user, command.params[i]);
			}
			else
				sendMsg(user.get_fd(), RPL_ENDOFNAMES(command.params[i]));
		}
	}
	else // print here all the names of the user
		user.servInfo->printAllChannelsUsers(user);
}

/***************** ListMinUser check if I enter "LIST >3" for example, I display the channels with at least 3 members **************/
bool	listMinUser(Command &command, User &user)
{
	(void) user;
	std::stringstream ss;
	if (command.params[0][0] == '>' && command.params.size() == 1)
	{
		if (command.params[0].size() == 1) // Means we only have ">"
			return (false);
		for (int i = 1; command.params[0][i] ; i++) // I check here if I only have digits
		{
			if (isdigit(command.params[0][i]) == false)
				return false;
		}
		user.servInfo->displayListMinUser(user, atoi(command.params[0].substr(1).c_str()) );
	}
	return (true);
}

/***************** LIST allows 
 * to show the channels
 * The numbers of users connected to the channel ont it
 * the TOPIC is displayed **************/
void	Command::list(Command &command, User &user)
{
	if (command.params.empty() == false)
	{
		if (listMinUser(command, user) == true)
			return;
		for (unsigned long i = 0 ; i < command.params.size()  ; i++)
		{
			if (user.servInfo->channelExist(command.params[i]) == true) 
			{
				Channel *chan = &user.servInfo->getChannel(command.params[i]);
				sendMsg(user.get_fd(), RPL_LIST(chan->getChannelName(), ft_itoa(chan->getNbUsers()), chan->getTopic()));
			}
			else
				sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[i]));
		}
		sendMsg(user.get_fd(), RPL_LISTEND(user.getNickName()));
	}
	else
		user.servInfo->printListChannels(user);
}


/***************** errors check
 *  * Conditions are the following ones: 
 * channel must exists
 * User invited must exists
 * The one who invites has to be on channel
 * The one who is invited must to not be on channel
 *  **************/
void	inviteErrorscheck(Command &command, User &user)
{
	if (user.servInfo->channelExist(command.params[1]) == true)
	{
		Channel *chan = &user.servInfo->getChannel(command.params[1]);
		if (chan->userInChannel(user.get_fd()) == true)
		{
			if (user.servInfo->userExist(command.params[0]) == true)
			{
				User *nick = &user.servInfo->nickToUserFd(command.params[0]);
				if (chan->userInChannel(nick->get_fd()) == false)
				{
					chan->addUserToChannel(nick->get_fd(), nick);
					sendMsg(nick->get_fd(), INVITE_WELCOME_MESSAGE(nick->getNickName(), chan->getChannelName()));
					sendMsg(user.get_fd(), RPL_INVITING(nick->getNickName(), chan->getChannelName()));
				}
				else
					sendMsg(user.get_fd(), ERR_USERONCHANNEL(nick->getNickName(), chan->getChannelName()));
			}
			else
				sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME());
		}
		else
			sendMsg(user.get_fd(), ERR_NOTOCHANNEL(user.getNickName(), chan->getChannelName()));
	}
	else
		sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[1]));
}

/***************** INVITE allows
 * To invite someone else to a channel you are already in
 * Conditions are the following ones: 
 * at lease 2 parameters : nickname and channel
 *  **************/
void	Command::invite(Command &command, User &user)
{
	if (command.params.size() == 2)
		inviteErrorscheck(command, user);
	else
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
}

/***************** Errors checks for KICK
 * 1: channel doesn't exist
 * 2: User kicked doesn't exist
 * 3: User kicked exists but not on the channel **************/
void	kickErrorCheck(Command &command, User &user)
{
	if (user.servInfo->channelExist(command.params[0]) == true)
	{
		Channel *chan = &user.servInfo->getChannel(command.params[0]);
		if (user.servInfo->userExist(command.params[1]) == true)
		{
			User *nick = &user.servInfo->nickToUserFd(command.params[1]);
			if (chan->userInChannel(nick->get_fd()) == true)
			{
				chan->removeUserChannel(nick->get_fd(), nick);
				if (command.params.size() == 3)
				{
					std::string message = command.params[2];
					sendMsg(nick->get_fd(), KICK__MESSAGE(user.getNickName(), chan->getChannelName(), message));
				}
				else if (command.params.size() == 2)
				{
					chan->removeUserChannel(nick->get_fd(), nick);
					sendMsg(nick->get_fd(), KICK__MESSAGE(user.getNickName(), chan->getChannelName(), "the host doesn't like you"));
				}
			}
			else
				sendMsg(user.get_fd(), ERR_USERNOTINCHANNEL(nick->getNickName(), chan->getChannelName()));
		}
		else
			sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME());
	}
	else
		sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[0]));
}

/***************** KICK allors
 * Need at least 2 params, the channel and the nickname
 * An optional message otherwise a default is set
 * having the right to kick **************/
void	Command::kick(Command &command, User &user)
{
	if (command.params.size() >= 2 && command.params.size() <= 3)
		kickErrorCheck(command, user);
	else
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
}