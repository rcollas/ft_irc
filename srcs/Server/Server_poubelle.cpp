#include "../../incs/Server/Server.hpp"

char Server::buffer[Server::bufferSize];

void	Server::fillAvailableCmd() {
	this->cmdList.push_back("CAP");
	this->cmdList.push_back("PASS");
	this->cmdList.push_back("NICK");
	this->cmdList.push_back("USER");
	this->cmdList.push_back("QUIT");
}

Server::Server()
:
serverEndPoint(0),
portNum("6667")
{
	int status;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	/**
	 * @brief addrinfo is a struct used to store various information used by the sockets interface
	 */

	if ((status = getaddrinfo("localhost", portNum.c_str(), &hints, &serverinfo)) != 0) {
		std::cerr << "getaddrinfo failed";
		exit(1);
	}
	serverAddress = (struct sockaddr_in *)serverinfo->ai_addr;
	std::cout << "Server IP = " << inet_ntoa(serverAddress->sin_addr) << std::endl; //inet_ntoa convert from network byte order to IPv4 number and dots
	this->fillAvailableCmd();

}

Server::~Server() {}

std::vector<std::string>	&Server::getCmdList() { return this->cmdList; }

void	Server::init()
{
	int yes = 1;
	serverEndPoint = socket(AF_INET, SOCK_STREAM, 0);
	if (serverEndPoint < 0)
	{
		std::cout << "Can't initiate socket." << std::endl;
		exit(1);
	}
	std::cout << "Server socket connection created..." << std::endl;

	setsockopt(serverEndPoint, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));


	if (bind(this->serverEndPoint, (struct sockaddr *)serverAddress, sizeof(*serverAddress)) < 0)
	{
		std::cout << "Error binding socket." << std::endl;
		std::cout << strerror(errno) << std::endl;
		exit(1);
	}
	socketSize = sizeof(*serverAddress);

	std::cout << "Looking for clients..." << std::endl;
	listen(serverEndPoint, 4);
	fcntl(serverEndPoint, O_NONBLOCK);
	pfds.push_back(pollfd());
	pfds[0].fd = serverEndPoint;
	pfds[0].events = POLLIN;

}

void	Server::cmdDispatcher(Command &cmd, User &user) {
	switch (cmd.cmd) {
		case (CAP): cmd.cap(cmd, user);
		case (PASS): cmd.pass(cmd, user);
		case (NICK): cmd.nick(cmd, user);
		case (USER): cmd.user(cmd, user);
	}
}

/**
 * @brief register a new user after a connection query
 * @param user User object used to store information (nickname, username...)
 * parse return a struct Command containing the command stored as an int
 * and the parameters in a vector
 */

void	Server::registration(User &user) {

	std::cout << "Registration in progress..." << std::endl;
	std::vector<std::string>	res;
	Command						*ret;
	sleep(1);
	recv(user.get_fd(), buffer, bufferSize, MSG_DONTWAIT);
	res = split(buffer, " \r\n");
	memset(buffer, 0, bufferSize);
	for (int i = 0; i < 4; i++) {
		ret = parse(res, user.servInfo->getCmdList());
		user.addCmd(*ret);
		printCmd(*ret);
	}
	while (user.getCmdList().empty() == false) {
		Server::cmdDispatcher(user.getCmdList().front(), user);
		user.getCmdList().erase(user.getCmdList().begin());
	}
	std::cout << user << std::endl;
	std::cout << "Registration complete!" << std::endl;
}

/**
 * @brief if the registration succeed, a set of numeric replies is sent to the clien
 * @param user used to retrieve user's information
 * @details numeric replies can be found in incs/Server/NumericReplies.hpp
 */

void Server::welcome(User &user) {

	sendMsg(user.get_fd(), RPL_WELCOME("localhost", user.getNickName(), user.getUserName()));
	send(user.get_fd(), RPL_YOURHOST().c_str(), RPL_YOURHOST().length(), 0);
	send(user.get_fd(), RPL_CREATED().c_str(),strlen(RPL_CREATED().c_str()), 0);
	send(user.get_fd(), RPL_MYINFO().c_str(),strlen(RPL_MYINFO().c_str()), 0);
	send(user.get_fd(), RPL_ISUPPORT().c_str(),strlen(RPL_ISUPPORT().c_str()), 0);
}

/**
 * @brief send a msg received from senderFd to all the clients on the server
 * @param senderFd the file descriptor of the sender
 * @param nbytes the number of bytes received from the sender
 */

void	Server::sendToAll(int senderFd, int nbytes) {

	for (int j = 0; j < (int)pfds.size(); j++)
	{
		int dest_fd = pfds[j].fd;

		if (dest_fd != serverEndPoint && dest_fd != senderFd)
		{
			if (send(dest_fd, buffer, nbytes, 0) == -1)
				perror("send");
			std::cout << "Sending: " << buffer << std::endl;
		}
	}
}

/**
 * @brief receive the client's request and either close the connection,
 * handle the request command or send a message to all the other users
 * @param i used to retrieve the pollfd struct of the sending client
 */

void	Server::handleClientRequest(int i) {

	std::cout << "client wants to communicate" << std::endl;
	int nbytes = recv(pfds[i].fd, buffer, bufferSize, MSG_DONTWAIT);
	int senderFd = pfds[i].fd;
	std::cout << "Receiving: " << buffer << std::endl;

	if (nbytes <= 0) {
		if (nbytes == 0) {
			std::cout << "pollserver: socket %d hung up " << senderFd << std::endl;
			user_list.erase(user_list.find(pfds[i].fd));
			pfds.erase(pfds.begin() + i);
		} else {
			perror("recv");
		}
	} else {
		this->sendToAll(senderFd, nbytes);
	}
	memset(buffer, 0, bufferSize);
}

/*std::string RPL_MOTDSTART(std::string server)
{
    std::string RPL_MOTDSTART;
    RPL_MOTDSTART += ":- " + server + " Message of the day - \r\n"; 
    return (RPL_MOTDSTART);
}

std::string RPL_MOTD()
{
	return (":- Welcome to our wonderful IRC server! \r\n");
}

std::string RPL_ENDOFMOTD()
{
	return (": End of /MOTD command\r\n");
}

std::string RPL_UNAWAY()
{
	return (  "<client> :You are no longer marked as being away");
}

std::string RPL_NOWAWAY()
{
	return ( "<client> You have been marked as being away\r\n");
}

std::string RPL_AWAY(char *buffer)
{
	if (strcmp(buffer, "AWAY\r\n") == 0)
	{
		return (RPL_UNAWAY());
	}
	if (strcmp(buffer, "AWAY :Je suis afk\r\n") == 0)
	{
		return (RPL_NOWAWAY() + "Je suis afk\r\n");
	}
	return ("Command not found\r\n");
}*/

void Server::run()
{
	while (true) {

		poll(pfds.data(), pfds.size(), -1);

		for (int i = 0; i < (int)pfds.size(); i++)
		{
			if (pfds[i].revents & POLLIN) // a socket is ready to listen
			{
				if (pfds[i].fd == serverEndPoint) {  // the socket is the entrepoint: new connection is requested
					User new_user(pfds, serverEndPoint, this);
					user_list.insert(std::pair<int, User>(new_user.get_fd(), new_user));
				} else { // a client wants to communicate
					std::cout << "client wants to communicate" << std::endl;
					int nbytes = recv(pfds[i].fd, buffer, bufferSize, MSG_DONTWAIT);
					int sender_fd = pfds[i].fd;
					std::cout << "Receiving: " << buffer << std::endl;
					/*if (strcmp(buffer, "motd\r\n") == 0)
					{
						send(new_fd, RPL_MOTDSTART("localhost").c_str(), strlen(RPL_MOTDSTART("localhost").c_str()), 0);
						send(new_fd, RPL_MOTD().c_str(), strlen(RPL_MOTD().c_str()), 0);
						send(new_fd, RPL_ENDOFMOTD().c_str(), strlen(RPL_ENDOFMOTD().c_str()), 0);
					}
					if (strcmp(buffer, "AWAY :Je suis afk\r\n") == 0)
					{
						send(new_fd, RPL_AWAY(buffer).c_str(), strlen(RPL_AWAY(buffer).c_str()), 0);
					}*/
					if (nbytes <= 0) {
						if (nbytes == 0) {
							std::cout << "pollserver: socket %d hung up " << sender_fd << std::endl;
						} else {
							perror("recv");
						}
					} else {
						for (int j = 0; j < (int)pfds.size(); j++) {
							int dest_fd = pfds[j].fd;

							if (dest_fd != serverEndPoint && dest_fd != sender_fd) {
								if (send(dest_fd, buffer, nbytes, 0) == -1) {
									perror("send");
								}
								std::cout << "Sending: " << buffer << std::endl;
							}
						}
					}
					memset(buffer, 0, bufferSize);
				}
			}
		}
	}

	for (int i = 0; (int)pfds.size(); i++) {
		close(pfds[i].fd);
	}
}