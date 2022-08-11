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
	else if (channelExist(command.params[0]) == true)
		std::cout << "\033[0;31m" << "Le channel exite déjà " << command.params[0] << "\033[0m" << std::endl;
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

// void	checkChannelCreation(char *buffer, int fd)
// {
// 	Channel *vincent = new Channel("#vincent", "0");
// 	// A modifier par un parsing ou je dois avoir 2 ou 3 param + toute la gestion d'erreur
// 	if (strcmp(buffer, "JOIN #vincent\r\n") == 0) 
// 	{
// 		std::cout << "WORKS joining Vincent" << std::endl;
// 		send(fd, JOINWELCOMEMESSAGE(vincent->getChannelName()).c_str(), strlen(JOINWELCOMEMESSAGE(vincent->getChannelName())).c_str(), 0);
// 		topicCommand(fd, buffer, vincent); // attendre du parsing de robin de pouvoir sortir mon channel vincent de cette fonction
// 	}
// 	else if (strcmp(buffer, "/JOIN\n") == 0) // à modifier par un parsing ou je dois avoir 2 ou 3 paramètres
// 		send(fd, ERR_NEEDMOREPARAMS().c_str(),strlen(ERR_NEEDMOREPARAMS().c_str()), 0);
// 	if (vincent->getChannelJoined() == true)
// 		topicCommand(fd, buffer, vincent); // attendre du parsing de robin de pouvoir sortir mon channel vincent de cette fonction
// }

// void	Server::checkChannel(char *buffer, int fd) // à enlever après parsing Robin On aura juste à checker 1 et 2 eme arg
// {
// 	checkChannelCreation(buffer, fd);
// }

//checkChannel(buffer, new_fd); // à enlever après parsing Robin
