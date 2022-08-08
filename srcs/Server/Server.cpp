#include "../../incs/Server/Server.hpp"


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

}

Server::~Server()
{}

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

void Server::welcome(int fd, std::string client_ip) {
		recv(fd, buffer, bufferSize, MSG_DONTWAIT);
		std::cout << buffer << std::endl;
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

struct in_addr get_in_addr(struct sockaddr *sa) {
	return ((struct sockaddr_in *)sa)->sin_addr;
}

std::string toString(char *str) {
	std::string s;
	while (*str) {
		s += *str++;
	}
	return s;
}

std::string RPL_MOTDSTART(std::string server)
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


void Server::run()
{
	(void)buffer;
	(void)socketSize;
	struct sockaddr_storage clientAddress;
	int new_fd;
	socklen_t sin_size;
	std::string client_ip;
	while (true) {
		poll(pfds.data(), pfds.size(), -1);
		sin_size = sizeof(clientAddress);
		for (int i = 0; i < (int)pfds.size(); i++) {
			if (pfds[i].revents & POLLIN) {  // a socket is ready to listen
				if (pfds[i].fd == serverEndPoint) {  // the socket is the entrepoint: new connection is requested
					new_fd = accept(serverEndPoint,
									(struct sockaddr *) &clientAddress,
									&sin_size);
					fcntl(new_fd, O_NONBLOCK);
					if (new_fd == -1) {
						perror("accept");
						continue;
					}
					/**
					 * @brief add a new pollfd to pfds. The poll struct's fd is set to new_fd (new client socket)
					 * we then greet the client with a welcome and show him his ip
					 */
					pfds.push_back(pollfd());
					pfds.back().fd = new_fd;
					pfds.back().events = POLLIN;
					client_ip = toString(inet_ntoa(
							(get_in_addr((struct sockaddr *) &clientAddress))));
					client_ip += "\r\n";
					std::cout << "client IP = " << client_ip << std::endl;
					welcome(new_fd, client_ip);
				} else { // a client wants to communicate
					std::cout << "client wants to communicate" << std::endl;
					int nbytes = recv(pfds[i].fd, buffer, bufferSize, MSG_DONTWAIT);
					int sender_fd = pfds[i].fd;
					std::cout << "Receiving: " << buffer << std::endl;
					if (strcmp(buffer, "motd\r\n") == 0)
					{
						std::cout << RPL_MOTDSTART("localhost");
						std::cout << RPL_MOTD();
						std::cout << RPL_ENDOFMOTD();
					}
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
		/**
		 * @brief to try it, open a terminal and type the following cmd : telnet localhost 6667
		 * you'll see:
		 * "hello"
		 * "127.0.0.1"
		 */

	}

	for (int i = 0; (int)pfds.size(); i++) {
		close(pfds[i].fd);
	}
}
