#include "../../incs/Server/Server.hpp"


/***************** Function to check and add a commit  **************/
void	topicCommand(int fd, char *buffer, Channel *vincent)
{
	(void) fd;
	if (strcmp(buffer, "TOPIC #vincent :bienvenue chez Vincent\r\n") == 0)
	{
		vincent->changeTopic("bienvenue chez Vincent\r\n"); // a modifier avec les arguments de robin
		send(fd, RPL_TOPIC(vincent->getChannelName().c_str(), vincent->getTopic()).c_str(),
			strlen(RPL_TOPIC(vincent->getChannelName().c_str(), vincent->getTopic()).c_str()), 0);
	}
	else if (strcmp(buffer, "TOPIC\r\n") == 0) // a tester avec nc car on reçoit pas la commande recv (/TOPIC)
	{
		if (vincent->getTopic() == "NOT WORKING")
		{
			std::cout << RPL_NOTOPIC(vincent->getChannelName()) << std::endl;
			send(fd, RPL_NOTOPIC(vincent->getChannelName()).c_str(),
				strlen(vincent->getChannelName().c_str()), 0);
		}
	}
	// else if (strcmp(buffer, "TOPIC :bienvenue chez Vincent\r\n") == 0) a tester sur NC avec un bon parsing
	// {
	// 	send(fd, ERR_NOTONCHANNEL(vincent->getChannelName()).c_str(), 
	// 		strlen(ERR_NOTONCHANNEL(vincent->getChannelName()).c_str()), 0);
	// }
	else
		std::cout << "topic don't work" << std::endl;

}

void	checkChannelCreation(char *buffer, int fd)
{
	Channel *vincent = new Channel("#vincent", "0");
	// A modifier par un parsing ou je dois avoir 2 ou 3 param + toute la gestion d'erreur
	if (strcmp(buffer, "JOIN #vincent\r\n") == 0) 
	{
		std::cout << "WORKS joining Vincent" << std::endl;
		send(fd, JOINWELCOMEMESSAGE(vincent->getChannelName()).c_str(), strlen(JOINWELCOMEMESSAGE(vincent->getChannelName()).c_str()), 0);
		topicCommand(fd, buffer, vincent); // attendre du parsing de robin de pouvoir sortir mon channel vincent de cette fonction
	}
	else if (strcmp(buffer, "/JOIN\n") == 0) // à modifier par un parsing ou je dois avoir 2 ou 3 paramètres
		send(fd, ERR_NEEDMOREPARAMS().c_str(),strlen(ERR_NEEDMOREPARAMS().c_str()), 0);
	if (vincent->getChannelJoined() == true)
		topicCommand(fd, buffer, vincent); // attendre du parsing de robin de pouvoir sortir mon channel vincent de cette fonction
}

void	Server::checkChannel(char *buffer, int fd) // à enlever après parsing Robin On aura juste à checker 1 et 2 eme arg
{
	checkChannelCreation(buffer, fd);
}

//checkChannel(buffer, new_fd); // à enlever après parsing Robin
