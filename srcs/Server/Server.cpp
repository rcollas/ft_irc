#include "../../incs/Server/Server.hpp"

Server::Server()
:
serverEndPoint(0),
portNum(1500)
{}

Server::~Server()
{}

int Server::init()
{
	serverEndPoint = socket(AF_INET, SOCK_STREAM, 0);
	if (serverEndPoint < 0)
	{
		std::cout << "Can't initiate socket." << std::endl;
		exit(1);
	}
	std::cout << "Server socket connection created..." << std::endl;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htons(INADDR_ANY);
	serverAddress.sin_port = htons(portNum);

	if (bind(this->serverEndPoint, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "Error binding socket." << std::endl;
		exit(1);
	}
	socketSize = sizeof(serverAddress);
	std::cout << "Looking for clients..." << std::endl;
	listen(serverEndPoint, 1024);
	return 0;
}

void	Server::addUser(int i)
{
	std::cout << "Adding user " << i << std::endl;
	pfds[0].fd = serverEndPoint;
	pfds[0].events = POLLIN;
	pfds.push_back(pollfd());
	pfds.back().fd = serverEndPoint;
	pfds.back().events = POLLIN;
}

void Server::run()
{
	(void)buffer;
	while (true) {
		socklen_t size = sizeof(serverAddress);
		int timeout = 0.1 * 60 * 1000;
		pfds.push_back(pollfd());
		pfds.back().events = POLLIN;
		std::cout << "pfds revents before = " << pfds.back().revents
				  << std::endl;
		pfds.back().fd = accept(serverEndPoint,
								(struct sockaddr *) &serverAddress, &size);
		int ret = poll(pfds.data(), pfds.size(), timeout);
		if (ret > 0) {
			recv(pfds[0].fd, buffer, bufferSize, 0);
			std::cout << buffer << std::endl;
			std::cout << "ret > 0" << std::endl;
		}
	}
	for (int i = 0; (int)pfds.size(); i++) {
		close(pfds[i].fd);
	}


//	int timeout = 3 * 60 * 1000;
//	for (int i = 0; i < (int)pfds.size(); i++) {
//		std::cout << "pdfs = " << pfds[i].fd << std::endl;
//	}
//	std::cout << "pdfs size = " << pfds.size() << std::endl;
//
//	do {
//		int ret = poll(pfds.data(), pfds.size(), timeout);
//		std::cout << "poll" << std::endl;
//		if (ret == -1)
//		{
//			std::cerr << "poll() failed" << std::endl;
//			exit(1);
//		} else if (ret == 0)
//		{
//			std::cout << "Waiting for connection" << std::endl;
//		}
//		else
//		{
//			for (int i = 0; i < (int)pfds.size(); i++) {
//				if (pfds[i].revents & POLLIN)
//					addUser(i);
//			}
//		}
//	} while (true);
	/*
	int i = 0;
	int newSd;
	do
	{
		int rc;
		int timeout = 3 * 60 * 1000;
		std::cout << "Waiting on poll()..." << std::endl;
		//pfds[0].fd = accept(serverEndPoint, (struct sockaddr*)&serverAddress, &socketSize);
		rc = poll(pfds.data(), pfds.size(), timeout);

		if (rc < 0)
		{
			std::cerr << "poll() failed" << std::endl;
			break;
		}
		if (rc == 0)
		{
			std::cout << "poll() timed out" << std::endl;
			break;
		}
		int currentSize = pfds.size() - 1;
		for (i = 0; i < currentSize; i++)
		{
			if (pfds[i].revents == 0)
				continue;
		}
		if (pfds[i].revents != POLLIN)
		{
			std::cerr << "Error! pdfs[" << i << "] = " << pfds[i].revents << std::endl;
			exit(1);
		}
		if (pfds[i].fd == serverEndPoint)
		{
			std::cout << "Listening socket is readable" << std::endl;
			do
			{
				newSd = accept(serverEndPoint, 0, 0);
				if (newSd < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "accept() failed" << std::endl;
						exit(1);
					}
					break;
				}
				std::cout << "Incoming connection: " << newSd << std::endl;
				pfds.push_back(pollfd());
				pfds.back().fd = newSd;
				pfds.back().events = POLLIN;
			} while (newSd != -1);
		}
		else
		{
			bool closeConnection = false;
			std::cout << "Descriptor " << pfds[i].fd << " is readable" << std::endl;

			do
			{
				rc = recv(pfds[i].fd, buffer, sizeof(buffer), 0);
				if (rc < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "recv() failed" << std::endl;
						closeConnection = true;
					}
					break;
				}
				if (rc == 0)
				{
					std::cout << "Connection closed" << std::endl;
					closeConnection = true;
					break;
				}
				int len = rc;
				std::cout << len << " bytes received" << std::endl;
				rc = send(pfds[i].fd, buffer, len, 0);
				if (rc < 0)
				{
					std::cerr << "send() failed" << std::endl;
					closeConnection = true;
					break;
				}
			} while (true);

			if (closeConnection)
			{
				close(pfds[i].fd);
				pfds.erase(pfds.begin() + i);
			}
		}
	} while (true);
	 */
}
