#include "../../incs/Server/Server.hpp"

/***************** Here I create a channel if it does not exit **************/
void		Server::createChannel(int fd, User *user, Command command) // a corriger
{
	if	(user->servInfo->channelExist(command.params[0]) == false)
	{
		std::cout << "\033[0;31m" << "Création du channel " << command.params[0] << "\033[0m" << std::endl;
		Channel *chan = new Channel(command.params[0], "");
		this->addChannel(chan);
	}
	else if (user->servInfo->channelExist(command.params[0]) == true)
		std::cout << "\033[0;31m" << "Le channel existe déjà " << command.params[0] << "\033[0m" << std::endl;
	(void) fd;
}

void	Server::addChannel(Channel *chan) { 
	this->allChan.insert(std::pair<std::string, Channel *>(chan->getChannelName(), chan)); 
}

bool	Server::channelExist(std::string chanName) {
	if (this->allChan.find(chanName) == this->allChan.end())
		return false;
	return true;
}

void	Server::printAllChannels(void)
{
	std::map<std::string, Channel *>::iterator it;
	it = this->allChan.begin();
	std::cout <<  "\033[0;31m" << "LA LIST DES CHANNELS EST LA SUIVANTE : " << "\033[0m"  << std::endl; // second sert à chercher la value de l'itérateur
	for (; it != this->allChan.end(); it++)
	{
		if (it->second)
			std::cout << "\033[0;31m" << it->second->getChannelName() << "\033[0m" << std::endl;
	}
}

Channel	&Server::getChannel(std::string chanName)
{
	std::map<std::string, Channel *>::iterator it;
	it = this->allChan.begin();
	for(; it != this->allChan.end(); it++)
	{
		if (it->second->getChannelName() == chanName)
		{
			return (*it->second);
		}
	}
	return (*it->second);
}

/***************** Function to check and add a commit  **************/
// void	topicCommand(int fd, char *buffer, Channel *vincent)
// {
// 	(void) fd;
// 	if (strcmp(buffer, "TOPIC #vincent :bienvenue chez Vincent\r\n") == 0)
// 	{
// 		vincent->changeTopic("bienvenue chez Vincent\r\n"); // a modifier avec les arguments de robin
// 		send(fd, RPL_TOPIC(vincent->getChannelName().c_str(), vincent->getTopic()).c_str(),
// 			strlen(RPL_TOPIC(vincent->getChannelName().c_str(), vincent->getTopic()).c_str()), 0);
// 	}
// 	else if (strcmp(buffer, "TOPIC\r\n") == 0) // a tester avec nc car on reçoit pas la commande recv (/TOPIC)
// 	{
// 		if (vincent->getTopic() == "NOT WORKING")
// 		{
// 			std::cout << RPL_NOTOPIC(vincent->getChannelName()) << std::endl;
// 			send(fd, RPL_NOTOPIC(vincent->getChannelName()).c_str(),
// 				strlen(vincent->getChannelName().c_str()), 0);
// 		}
// 	}
// 	// else if (strcmp(buffer, "TOPIC :bienvenue chez Vincent\r\n") == 0) a tester sur NC avec un bon parsing
// 	// {
// 	// 	send(fd, ERR_NOTONCHANNEL(vincent->getChannelName()).c_str(), 
// 	// 		strlen(ERR_NOTONCHANNEL(vincent->getChannelName()).c_str()), 0);
// 	// }
// 	else
// 		std::cout << "topic don't work" << std::endl;

// }

