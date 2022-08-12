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
	(void)command;
	send(user.get_fd(), "PASS\r\n", strlen("PASS\r\n"), 0);
}

void	Command::nick(Command &command, User &user) {
	if (command.params.size() > 1)
		return ;
	else if (command.params.empty() == true)
		sendMsg(user.get_fd(), ERR_NONICKNAMEGIVEN());
	else {
		if (user.getNickName() == command.params[0])
			sendMsg(user.get_fd(), ERR_NICKNAMEINUSE(user.getNickName()));
		else if (command.params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_-") != std::string::npos)
			sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME());
		else {
			if (user.getNickName().empty() == true) {
				user.set_nickname(command.params[0]);
				return ;
			}
			user.servInfo->sendToAll(user.get_fd(), user.getNickName() + "!@localhost NICK " + command.params[0] + "\n");
			user.set_nickname(command.params[0]);
		}
	}
}

void	Command::user(Command &command, User &user) {
	std::string msg = "USER " + user.getUserName() + "\r\n";
	if (user.getUserName().empty() == false) {
		user.set_username(command.params[0]);
		send(user.get_fd(), msg.c_str(), msg.length(), 0);
	} else {
		user.set_username(command.params[0]);
	}
}

void	Command::motd(Command &command, User &user) {
	if (command.params.empty() == true) {
		sendMsg(user.get_fd(), RPL_MOTDSTART(user.getNickName(), "localhost"));
		sendMsg(user.get_fd(), RPL_MOTD(user.getNickName(), "Welcooooooooooome at home!"));
		sendMsg(user.get_fd(), RPL_ENDOFMOTD(user.getNickName()));
	}
}

void	Command::away(Command &command, User &user) {
	if (command.params.empty() == true) {
		user.set_isAway(false);
		sendMsg(user.get_fd(), RPL_UNAWAY(user.getNickName()));
	}
	if (command.params.empty() == false){
		user.set_isAway(true);
		sendMsg(user.get_fd(), RPL_NOWAWAY(user.getNickName()));
		sendMsg(user.get_fd(), RPL_AWAY(user.getNickName(), command.params[0]));
	}
}

void	Command::version(Command &command, User &user){
	if (command.params.empty() == true) {
		sendMsg(user.get_fd(), RPL_VERSION(user.getNickName()));
	}
}

void	Command::lusers(Command &command, User &user)
{
	if (command.params.empty() == true)
	{
		sendMsg(user.get_fd(), RPL_LUSERCLIENT(user.getNickName(), ft_itoa(user.servInfo->getNbOfUsers())));
		sendMsg(user.get_fd(), RPL_LUSEROP(user.getNickName(), "0"));
		sendMsg(user.get_fd(), RPL_LUSERCHANNELS(user.getNickName(), "getNumberOfChan"));
		//sendMsg(user.get_fd(), RPL_LUSERME(user.getNickName(), ft_itoa(user.servInfo->getNbOfUsers())));
	}
}

/***************** CHANNEL COMMAND **************/

void	Command::join(Command &command, User &user) {
	if (command.params.empty() == false)
	{
		user.servInfo->createChannel(user.get_fd(), user, command);
		user.servInfo->printAllChannels(); // to remove
		for (unsigned long i = 0 ; i < command.params.size() ; i++)
		{
			Channel *chan = &user.servInfo->getChannel(command.params[i]);
			chan->addUserToChannel(user.get_fd(), &user);
			chan->printChannelUsers(user.get_fd(), &user); // to remove
			user.servInfo->printWelcomeMessage(user.get_fd(), user, command, chan);
			if (chan->TopicIsSet() == true)
			{
				std::cout << "\e[0;34m" << "I SHOULD SEND TOPIC TO CLIENT" <<  "\033[0m" << std::endl; // A REMOVE
				std::cout << "\e[0;34m" << chan->TopicIsSet() <<  "\033[0m" << std::endl;
				sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
			}
			else
				sendMsg(user.get_fd(), RPL_NOTOPIC(chan->getChannelName()));
		}
	}
	else 
	{
		std::cout << "\e[0;34m" << ERR_NEEDMOREPARAMS(user.getNickName()) <<  "\033[0m" << std::endl; // A REMOVE
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}



/***************** I get the channel and I set the topic here to send to the client I need to have at least 3 param TOPIC <channel> <message> **************/
void	SetTopic(Command &command, User &user)
{
	Channel *chan = &user.servInfo->getChannel(command.params[0]);
	if (command.params.size() >= 2 && chan->userInChannel(user.get_fd()) == true)
	{
		std::string topic;
		for (unsigned long i = 1; i <= command.params.size(); i++)
			topic = concatenate_strings(topic, command.params[i]);
		std::cout << "\e[0;34m" << "Je suis dans le bon TOPIC" << chan->getTopic() <<  "\033[0m" << std::endl; // A REMOVE
		chan->changeTopic(topic);
		sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
	}
}


void	sendTopic(Command &command, User &user)
{
	Channel *chan = &user.servInfo->getChannel(command.params[0]);
	//std::cout << "\e[0;34m" << "Je suis dans le bon TOPIC" << chan->getTopic() <<  "\033[0m" << std::endl; // A REMOVE

	if (command.params.size() == 1 && chan->userInChannel(user.get_fd()) == true && chan->TopicIsSet() == true)
	{
		std::cout << "\e[0;34m" << "Je suis dans le bon TOPIC" << chan->getTopic() <<  "\033[0m" << std::endl; // A REMOVE
		sendMsg(user.get_fd(), RPL_TOPIC(user.getNickName(), chan->getChannelName(), chan->getTopic()));
	}
	else if (command.params.size() == 1 && chan->userInChannel(user.get_fd()) == true && chan->TopicIsSet() == false)
	{
		std::cout << "\e[0;34m" << "Je suis dans le bon TOPIC" << chan->getTopic() <<  "\033[0m" << std::endl; // A REMOVE
		sendMsg(user.get_fd(), RPL_NOTOPIC(chan->getChannelName()));
	}
}
/***************** I handle the topic command who set a topic for a channel **************/
void	Command::topic(Command &command, User &user) {
	if (command.params.empty() == false)
	{
		if (user.servInfo->channelExist(command.params[0]) == true) //I verify if param[1](chanName exist)
		{
				SetTopic(command, user);
				sendTopic(command, user);
		}
		else if (user.servInfo->channelExist(command.params[0]) == false) // if the channel doesn't exist = error
		{
			std::cout << "\e[0;34m" << ERR_NOSUCHCHANNEL(user.getNickName(), command.params[0]) <<  "\033[0m" << std::endl; // A REMOVE
			sendMsg(user.get_fd(), ERR_NOSUCHCHANNEL(user.getNickName(), command.params[0]));
		}
	}
	else 
	{
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}

/***************** PART remove a user from one+ channels **************/
// first condition : I check if channel exist and i am in the channel
//  second condition : I check if channel exist and i am not in the channel
 // thirs condition the channel doesn't exist
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
					chan->printChannelUsers(user.get_fd(), &user);
					chan->removeUserChannel(user.get_fd(), &user);
					chan->printChannelUsers(user.get_fd(), &user);
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
		std::cout << "\e[0;34m" << ERR_NEEDMOREPARAMS(user.getNickName()) <<  "\033[0m" << std::endl; // A REMOVE
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
	}
}