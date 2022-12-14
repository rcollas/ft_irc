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
	this->isAway = false;
	this->isOperator = false;
	this->modesNumber = 0;
	this->servInfo = serverInfo;
	this->validPasswd = false;
	socklen_t	sin_size = sizeof(socket);

	fd = accept(serverEndpoint,
					(struct sockaddr *) &socket,
					&sin_size);

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
	std::cout << "New client connection. Fd: " << pfds.back().fd << std::endl;
}

User::~User() {}

int User::get_fd() const { return this->fd; }

void	User::set_username(std::string username) { this->user_name = username; }
void	User::set_nickname(std::string nickname) { this->nick_name = nickname; }
void	User::set_realname(std::string realname) { this->real_name = realname; }
void	User::set_quitMessage(std::string quitMessage) { this->quitMessage = quitMessage; }
void	User::set_awayMessage(std::string awayMessage) { this->awayMessage = awayMessage; }
void	User::set_isAway(bool away) { this->isAway = away; }
void	User::set_isOperator(bool isOperator) { this->isOperator = isOperator; }
void	User::set_modesNumber(int modes) { this->modesNumber = modes; }
void	User::setPasswdStatus(bool status) { this->validPasswd = status; };
void	User::appendToBuffer(std::string str) { this->buffer.append(str); }
void	User::clearBuffer() { this->buffer.clear(); }

std::string	User::getNickName() const { return this->nick_name; }
std::string	User::getUserName() const { return this->user_name; }
std::string	User::getRealName() const { return this->real_name; }
std::string	User::getAwayMessage() const { return this->awayMessage; }
std::string	User::getBuffer() const { return this->buffer; };
std::string	User::getQuitMessage() const { return this->quitMessage; };
bool		User::getIsAway() const { return this->isAway; }
bool		User::getIsOperator() const { return this->isOperator; }
int			User::getModesNumber() const { return this->modesNumber; }
bool		User::isAdmin() const { return this->admin; }
bool		User::bringValidPasswd() const { return this->validPasswd; };

void		User::addCmd(Command &cmd) { this->cmds.push_back(cmd); }
void		User::addChan(Channel *chan) { this->activeChan.push_back(chan); }
std::vector<Command>	&User::getCmdList() { return this->cmds; }
std::vector<Channel *>  User::getActiveChan() { return this->activeChan; }

std::ostream &operator<<(std::ostream &out, User &user) {
	out << "User information: " << std::endl;
	out << "Nick: " << user.getNickName() << std::endl;
	out << "User: " << user.getUserName();
	out << "Real name: " << user.getRealName();
	return out;
}

void User::deleteAllChannelUsers(User &user, Command &command)
{
	std::vector<Channel *>::iterator it;
	std::vector<Channel *> chan = user.getActiveChan();
	(void) command;
	it = chan.begin();
	for (; it != chan.end(); it++)
	{
		(*it)->removeUserChannel(user.get_fd(), &user, "");
	}
}

void User::deleteQuitChannelUsers(User &user, Command *command)
{
	std::vector<Channel *>::iterator it;
	std::vector<Channel *> chan = user.getActiveChan();
	it = chan.begin();
	for (; it != chan.end(); it++)
	{
		(*it)->removeUserChannel(user.get_fd(), &user, "");
		command->sendMessageToChannel(user, (*it)->getChannelName(), "\033[0;31m" + user.getNickName() + "!@localhost QUIT: " + user.getQuitMessage() + "\r\n\033[0m");
	}
}