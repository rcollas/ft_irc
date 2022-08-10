#include "../../incs/User/User.hpp"


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


User::User(std::vector<pollfd> &pfds, int serverEndpoint, Server *serverInfo) {

	this->servInfo = serverInfo;
	socklen_t	sin_size = sizeof(socket);

	fd = accept(serverEndpoint,
					(struct sockaddr *) &socket,
					&sin_size);

	fcntl(fd, O_NONBLOCK);

	if (fd == -1) {
		perror("accept");
	}
	/**
	 * @brief add a new pollfd to pfds. The poll struct's fd is set to fd (new client socket)
	 * we then greet the client with a set of numeric replies and show him his ip
	 */
	pfds.push_back(pollfd());
	pfds.back().fd = fd;
	pfds.back().events = POLLIN;
	std::string client_ip = toString(inet_ntoa(
			(get_in_addr((struct sockaddr *) &socket))));
	client_ip += "\r\n";
	std::cout << "client IP = " << client_ip << std::endl;
	Server::registration(*this);
	Server::welcome(*this);
}

User::~User() {}

int User::get_fd() { return this->fd; }

void	User::set_username(std::string username) { this->user_name = username; }
void	User::set_nickname(std::string nickname) { this->nick_name = nickname; }

std::string	User::getNickName() const { return this->nick_name; }
std::string	User::getUserName() const { return this->user_name; }

void	User::addCmd(Command &cmd) { this->cmds.push_back(cmd); }
void	User::addChan(Channel *chan) { this->activeChan.push_back(chan); }
std::vector<Command>	&User::getCmdList() { return this->cmds; }

std::ostream &operator<<(std::ostream &out, User &user) {
	out << "User information: " << std::endl;
	out << "Nick: " << user.getNickName() << std::endl;
	out << "User: " << user.getUserName();
	return out;
}
