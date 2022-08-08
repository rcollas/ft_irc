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

void Server::welcome(int fd, std::string client_ip) {
	//std::string cap = "CAP\r\n";
	std::string nick = "NICK rcollas\r\n";
	std::string user = "USER guest 0 * :Robin COllas\r\n";
	//send(fd, cap.c_str(), cap.length(), 0);
	send(fd, nick.c_str(), nick.length(), 0);
	send(fd, user.c_str(), user.length(), 0);
	send(fd, RPL_WELCOME().c_str(),strlen(RPL_WELCOME().c_str()), 0);
	send(fd, RPL_YOURHOST().c_str(), strlen(RPL_YOURHOST().c_str()), 0);
	send(fd, RPL_CREATED().c_str(),strlen(RPL_CREATED().c_str()), 0);
	send(fd, RPL_MYINFO().c_str(),strlen(RPL_MYINFO().c_str()), 0);
	send(fd, RPL_ISUPPORT().c_str(),strlen(RPL_ISUPPORT().c_str()), 0);
	//send(fd, "CAP\r\n", strlen("CAP\r\n"), 0);
	(void)client_ip;
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

/**
 * @brief run handles connection and client's requests
 * @details the poll function is used to check the state of the open sockets using poll and the struct pollfd
 * if a client wants to communicate, pfds.revent is set accordingly (POLLIN for writing, POLLOUT for reading)
 * if the pfds.fd correspond to the serverEndpoint, a new connection is requested
 */

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
					this->handleClientRequest(i);
				}
			}
		}
	}

	for (int i = 0; (int)pfds.size(); i++) {
		close(pfds[i].fd);
	}
}
