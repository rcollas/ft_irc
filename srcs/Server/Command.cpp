#include "../../incs/ft_irc.h"
#include "../../incs/Server/Server.hpp"


void	Command::cap(Command &command, User &user) {
	(void)command;
	send(user.get_fd(), "CAP\r\n", strlen("CAP\r\n"), 0);
}

void	Command::pass(Command &command, User &user) {
	(void)command;
	send(user.get_fd(), "PASS\r\n", strlen("PASS\r\n"), 0);
}

void	Command::nick(Command &command, User &user) {
	std::string msg = "NICK " + user.getNickName() + "\r\n";
	if (user.getNickName().empty() == false) {
		user.set_nickname(command.params[0]);
		send(user.get_fd(), msg.c_str(), msg.length(), 0);
	} else {
		user.set_nickname(command.params[0]);
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

/*void	Command::motd(Command &command, User &user) {

}

void	Command::away(Command &command, User &user) {

}*/
/***************** CHANNEL COMMAND **************/

void	Command::join(Command &command, User &user) {
	if (command.params[0].length())
	{
		user.servInfo->createChannel(user.get_fd(), &user, command);
		user.servInfo->printAllChannels();
		Channel *chan = &user.servInfo->getChannel(command.params[0]);
		chan->addUserToChannel(user.get_fd(), &user);
		chan->printChannelUsers(user.get_fd(), &user);
		sendMsg(user.get_fd(), JOIN_WELCOME_MESSAGE(user.getNickName(), chan->getChannelName()));
	}
	else 
	{
		std::cout << "\e[0;34m" << ERR_NEEDMOREPARAMS(user.getNickName()) <<  "\033[0m" << std::endl; // A REMOVE
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
		return;
	}
}

/***************** I handle the topic command who set a topic for a channe **************/
void	Command::topic(Command &command, User &user) {
	if (command.params[0].length() && command.params[1].length())
	{
		// JE CHECK SI il y a un 2ème paramètre
		// JE SET LE TOPIC
		if (user.servInfo->channelExist(command.params[0]) == true) //I verify if param[1](chanName exist)
		{
			Channel *chan = &user.servInfo->getChannel(command.params[0]);
			if (chan->userInChannel(user.get_fd()) == true)	
			{
				std::cout << "\e[0;32m" << "I CAN SET THE TOPICCC" <<  "\033[0m" << std::endl;
			}
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
