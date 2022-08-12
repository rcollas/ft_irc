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
			if (user.getNickName().empty() == true && user.servInfo->channelExist(command.params[0]) == false) {
				user.set_nickname(command.params[0]);
				std::cout << "Nickname is set at " << command.params[0] << std::endl;
				return ;
			}
			if(user.servInfo->channelExist(command.params[0]) == false) {
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
	else if (command.params.size() < 4) {
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
		return ;
	}
	if (!(command.params[1] == "0" && command.params[2] == "*")) {
		std::cout << "Usage: /USER <username> 0 * <realname>\n";
		return ;
	}	
	if (user.getUserName().empty() == false) {
		sendMsg(user.get_fd(), ERR_ALREADYREGISTERED(user.getNickName()));
		return ;
	}
	if (command.params[3].length() < 1 || command.params[3].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
		std::cout << "Realname contains invalid characters\n";
		return ;
	}
	user.set_username(command.params[0]);
	std::cout << "Username is set at " << command.params[0] << std::endl;
	user.set_realname(command.params[3]);
	std::cout << "Realname is set at " << command.params[3] << std::endl;
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

/***************** I handle the topic command who set a topic for a channel **************/
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
