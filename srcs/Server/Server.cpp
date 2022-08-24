#include "../../incs/Server/Server.hpp"

char Server::buffer[Server::bufferSize];

void	Server::fillAvailableCmd() {
	this->cmdList.push_back("CAP");
	this->cmdList.push_back("PASS");
	this->cmdList.push_back("NICK");
	this->cmdList.push_back("USER");
	this->cmdList.push_back("JOIN");
	this->cmdList.push_back("TOPIC");
	this->cmdList.push_back("QUIT");
	this->cmdList.push_back("motd");
	this->cmdList.push_back("AWAY");
	this->cmdList.push_back("version");
	this->cmdList.push_back("lusers");
	this->cmdList.push_back("PART");
	this->cmdList.push_back("NAMES");
	this->cmdList.push_back("LIST");
	this->cmdList.push_back("INVITE");
	this->cmdList.push_back("KICK");
	this->cmdList.push_back("PRIVMSG");
	this->cmdList.push_back("MODE");
	this->cmdList.push_back("OPER");
	this->cmdList.push_back("NOTICE");
	this->cmdList.push_back("QUIT");
}

Server::Server(std::string port, std::string passwd)
:
serverEndPoint(0),
portNum(port),
password(passwd),
nbOfOperators(0),
serverPassword("4321")
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
std::vector<std::string>	&Server::getInvisibleList() { return this->invisibleList; }
std::string	Server::getPassword() const { return this->password; }


void	Server::setPortNum(std::string portNum) { this->portNum = portNum; }


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
	pfds.push_back(pollfd());
	pfds[0].fd = serverEndPoint;
	pfds[0].events = POLLIN;
	freeaddrinfo(serverinfo);

}

User	*Server::getUser(int userFd) {
	std::map<int, User>::iterator res = user_list.find(userFd);
	if (user_list.find(userFd) != user_list.end()) {
		return &res->second;
	}
	return &res->second;
}

int	Server::getNbOfUsers() { return this->user_list.size(); }

std::vector<std::string>	Server::getAdmin() {
	std::vector<std::string>	adminList;
	for (std::map<int, User>::iterator it = user_list.begin()
			; it != user_list.end(); it++) {
		if (it->second.isAdmin()) {
			adminList.push_back(it->second.getUserName());
		}
	}
	return adminList;
}

bool	Server::nicknameExists(std::string nickname) {
	std::map<int, User>::iterator it;
	it = this->user_list.begin();
	for(; it != this->user_list.end(); it++)
	{
		if (it->second.getNickName() == nickname)
		{
			return (true);
		}
	}
	return (false);
}

std::string	Server::getServerPassword() { return this->serverPassword; }

int	Server::getTargetFd(std::string nickname) {
	std::map<int, User>::iterator it;
	it = this->user_list.begin();
	for(; it != this->user_list.end(); it++)
	{
		if (it->second.getNickName() == nickname)
		{
			return (it->second.get_fd());
		}
	}
	return (it->second.get_fd());
}

bool	Server::getAwayStatus(std::string nickname) {
	std::map<int, User>::iterator it;
	it = this->user_list.begin();
	for(; it != this->user_list.end(); it++)
	{
		if (it->second.getNickName() == nickname)
		{
			return (it->second.getIsAway());
		}
	}
	return (it->second.getIsAway());
}

int	Server::getModes(std::string nickname) {
	std::map<int, User>::iterator it;
	it = this->user_list.begin();
	for(; it != this->user_list.end(); it++)
	{
		if (it->second.getNickName() == nickname) {
			return (it->second.getModesNumber());
		}
	}
	return (it->second.getModesNumber());
}

std::string	Server::getAwayString(std::string nickname) {
	std::map<int, User>::iterator it;
	it = this->user_list.begin();
	for(; it != this->user_list.end(); it++)
	{
		if (it->second.getNickName() == nickname)
		{
			return (it->second.getAwayMessage());
		}
	}
	return (it->second.getAwayMessage());
}

bool	Server::usernameExists(std::string username) {
	std::map<int, User>::iterator it;
	it = this->user_list.begin();
	for(; it != this->user_list.end(); it++)
	{
		if (it->second.getUserName() == username)
		{
			return (true);
		}
	}
	return (false);
}

void	Server::killConnection(User &user) {
	close(user.get_fd());
	for (size_t i = 0; i < pfds.size(); i++) {
		if (pfds[i].fd == user.get_fd()) {
			pfds.erase(pfds.begin() + i);
			break;
		}
	}
}

bool	registrationComplete(User &user) {
	return !user.getNickName().empty() && !user.getUserName().empty();
}

bool	cmdIsComplete(std::string cmd) {
	return cmd.find("\r\n") != cmd.npos;
}

void	Server::cmdDispatcher(Command &cmd, User &user) {
	//int ret = 1;
	if (cmdIsComplete(user.getBuffer())) {
		std::cout << "dispatch" << std::endl;
		switch (cmd.cmd) {
			case (PASS): cmd.pass(cmd, user); break;
		}
		if (user.bringValidPasswd() == false && cmd.cmd != CAP) {
			cmd.error("Fatal error: no password provided", user);
		} else {
			switch (cmd.cmd) {
				case (NICK): cmd.nick(cmd, user); break;
				case (USER): cmd.user(cmd, user); break;
			}
			if (registrationComplete(user) == true) {
				switch (cmd.cmd) {
					case (JOIN): cmd.join(cmd, user); break;
					case (TOPIC): cmd.topic(cmd, user); break;
					case (MOTD): cmd.motd(cmd, user); break;
					case (AWAY): cmd.away(cmd, user); break;
					case (VERSION): cmd.version(cmd, user); break;
					case (LUSERS): cmd.lusers(cmd, user); break;
					case (PART): cmd.part(cmd, user); break;
					case (NAMES): cmd.names(cmd, user); break;
					case (LIST): cmd.list(cmd, user); break;
					case (INVITE): cmd.invite(cmd, user); break;
					case (KICK): cmd.kick(cmd, user); break;
					case (PRIVMSG): cmd.privmsg(cmd, user); break;
					case (MODE): cmd.mode(cmd, user); break;
					case (OPER): cmd.oper(cmd, user); break;
					case (NOTICE): cmd.notice(cmd, user); break;
					case (QUIT): cmd.quit(cmd, user); break;
					//default: ret = 0;
				}
			} else if (cmd.cmd != CAP && cmd.cmd != PASS && cmd.cmd != USER && cmd.cmd != NICK) {
				sendMsg(user.get_fd(), ERR_NOTREGISTERED(user.getNickName()));
			}
		}
		//user.clearBuffer();
	}

}

User &Server::nickToUserFd(std::string nickname)
{
	std::map<int, User>::iterator it = this->user_list.begin();
	for (; it != this->user_list.end(); it++) {
		if (it->second.getNickName() == nickname)
			return it->second;
	}
	return (it->second);
}

bool	Server::userExist(std::string nickName)
{
	std::map<int, User>::iterator it = this->user_list.begin();
	for (; it != this->user_list.end(); it++) {
		if (it->second.getNickName() == nickName)
			return true;
	}
	return false;
}

int	Server::get_isOperatorStatus()
{
	int count = 0;
	std::map<int, User>::iterator it = this->user_list.begin();
	for (; it != this->user_list.end(); it++) {
		if (it->second.getIsOperator() == true) {
			count++;
		}
	}
	return count;
}

/**
 * @brief if the registration succeed, a set of numeric replies is sent to the client
 * @param user used to retrieve user's information
 * @details numeric replies can be found in incs/Server/NumericReplies.hpp
 */

void Server::welcome(User &user) {

	sendMsg(user.get_fd(), RPL_WELCOME("localhost", user.getNickName(), user.getUserName()));
	sendMsg(user.get_fd(), RPL_YOURHOST(user.getNickName()));
	sendMsg(user.get_fd(), RPL_CREATED(user.getNickName()));
	sendMsg(user.get_fd(), RPL_MYINFO(user.getNickName()));
	sendMsg(user.get_fd(), RPL_ISUPPORT(user.getNickName()));
}

/**
 * @brief send a msg received from senderFd to all the clients on the server
 * @param senderFd the file descriptor of the sender
 * @param nbytes the number of bytes received from the sender
 */

void	Server::sendToAll(int senderFd, std::string msg) {
	for (int j = 0; j < (int)pfds.size(); j++)
	{
		int dest_fd = pfds[j].fd;

		if (dest_fd != serverEndPoint && dest_fd != senderFd)
		{
			sendMsg(dest_fd, msg);
		}
	}
}

void	Server::handleCmd(User *user) {
	user->appendToBuffer(buffer);
	std::cout << "user buffer = " << user->getBuffer() << std::endl;
	std::vector<std::string>	res = split(user->getBuffer(), " :\r\n");
	Command						ret;
	memset(buffer, 0, bufferSize);
	while (res.empty() == false) {
		ret = parse(res, user->servInfo->getCmdList());
		user->addCmd(ret);
	}
	while (user->getCmdList().empty() == false) {
		Server::cmdDispatcher(user->getCmdList().front(), *user);
		if (user->getCmdList().front().cmd == QUIT) {
			user_list.erase(user_list.find(user->get_fd()));
			return ;
		}
		user->getCmdList().erase(user->getCmdList().begin());
	}
	if (cmdIsComplete(user->getBuffer()))
		user->clearBuffer();
}

/**
 * @brief receive the client's request and either close the connection,
 * handle the request command or send a message to all the other users
 * @param i used to retrieve the pollfd struct of the sending client
 */

void	Server::handleClientRequest(int i) {

	int nbytes = recv(pfds[i].fd, buffer, bufferSize, MSG_DONTWAIT);
	int senderFd = pfds[i].fd;

	if (nbytes <= 0) {
		if (nbytes == 0) {
			std::cout << "pollserver: socket " << senderFd << " hung up" << std::endl;
			killConnection(*getUser(pfds[i].fd));
			user_list.erase(user_list.find(pfds[i].fd));
		} else {
			perror("recv");
		}
	} else {
		this->handleCmd(getUser(senderFd));
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
	while (serverIsRunning) {
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
	for (size_t i = 1; i < pfds.size(); i++) {
		sendMsg(pfds[i].fd, "Server closed the connection");
		this->killConnection(*getUser(pfds[i].fd));
	}
	for (std::map<std::string, Channel *>::iterator it = allChan.begin(); it != allChan.end(); it++) {
		delete it->second;
	}
	close(serverEndPoint);
	cmdList.clear();
	pfds.clear();
	std::cout << "\nServer killed\n";
}

int	Server::getNbOfChan()
{
	return (this->allChan.size());
}