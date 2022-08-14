#include "../../incs/Server/Server.hpp"

/***************** Here I create a channel if it does not exit **************/
void		Server::createChannel(int fd, User &user, Command &command) // a corriger
{
	for (unsigned long i = 0; i < command.params.size(); i++)
	{
		if	(user.servInfo->channelExist(command.params[i]) == false)
		{
			std::cout << "\033[0;31m" << "Création du channel " << command.params[i] << "\033[0m" << std::endl;
			Channel *chan = new Channel(command.params[i], "");
			this->addChannel(chan);
		}
		else if (user.servInfo->channelExist(command.params[i]) == true)
			std::cout << "\033[0;31m" << "Le channel existe déjà " << command.params[i] << "\033[0m" << std::endl;
	(void) fd;
	}
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

void Server::printWelcomeMessage(int fd, User &user, Command &command, Channel *chan)
{
	(void) command;
	
		sendMsg(fd, JOIN_WELCOME_MESSAGE(user.getNickName(), chan->getChannelName()));
}

void Server::printAllChannelsUsers(User &user)
{
	std::map<std::string, Channel *>::iterator it;
	it = this->allChan.begin();
	for (; it != this->allChan.end(); it++)
	{
		it->second->printChannelUsers(user.get_fd(), &user, it->second->getChannelName());
	}
}

void Server::printListChannels(User &user)
{
	std::map<std::string, Channel *>::iterator it;
	it = this->allChan.begin();
	for (; it != this->allChan.end(); it++)
		sendMsg(user.get_fd(), RPL_LIST(it->second->getChannelName(), ft_itoa(it->second->getNbUsers()), it->second->getTopic()));
	sendMsg(user.get_fd(), RPL_LISTEND(user.getNickName()));
}

void Server::displayListMinUser(User &user, int minUser)
{
	std::map<std::string, Channel *>::iterator it;
	it = this->allChan.begin();
	for (; it != this->allChan.end(); it++)
	{
		if (it->second->getNbUsers() >= minUser)
			sendMsg(user.get_fd(), RPL_LIST(it->second->getChannelName(), ft_itoa(it->second->getNbUsers()), it->second->getTopic()));
	}
	sendMsg(user.get_fd(), RPL_LISTEND(user.getNickName()));
}