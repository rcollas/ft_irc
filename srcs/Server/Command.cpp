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
		// ajouter condition si le channel existe déjà
		user.servInfo->createChannel(user.get_fd(), &user, command);
		user.servInfo->printAllChannels();
		Channel *chan = &user.servInfo->getChannel(command.params[0]);
		chan->addUserToChannel(user.get_fd(), &user);
		std::cout <<  "\033[0;31m" << chan->getChannelName() <<  " WORKS HARD" << "\033[0m" << std::endl;
		chan->printChannelUsers(user.get_fd(), &user);
		//send(user.get_fd(), JOINWELCOMEMESSAGE(chan->getChannelName(),
			//user.getUserName()).c_str(), 
			//strlen(JOINWELCOMEMESSAGE(chan->getChannelName(), user.getUserName()).c_str()), 0);
	}
	else 
	{
		send(user.get_fd(), ERR_NEEDMOREPARAMS().c_str(),
			strlen(ERR_NEEDMOREPARAMS().c_str()), 0);
		return;
	}
}
