#include "../../incs/Server/Server.hpp"

/***************** Here I create a channel if it does not exit **************/
void		Server::createChannel(User &user, std::string chanName) // a corriger
{
	if	(user.servInfo->channelExist(chanName) == false)
	{
		std::cout << "\033[0;31m" << "Création du channel " << chanName << "\033[0m" << std::endl;
		Channel *chan = new Channel(chanName, "");
		this->addChannel(chan);
	}
	else
		std::cout << "\033[0;31m" << "Le channel existe déjà " << chanName << "\033[0m" << std::endl;
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

void Server::deleteAllChannelUsers(User &user)
{
	std::vector<Channel *>::iterator it;
	std::vector<Channel *> chan = user.getActiveChan();
	it = chan.begin();
	for (; it != chan.end(); it++)
	{
		
		it->first->getChannelName();
		
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