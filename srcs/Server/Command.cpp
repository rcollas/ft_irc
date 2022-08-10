#include "../../incs/ft_irc.h"

void	Command::cap(Command &command, User &user) {
	(void)command;
	send(user.get_fd(), "CAP\r\n", strlen("CAP\r\n"), 0);
}

void	Command::pass(Command &command, User &user) {
	(void)command;
	send(user.get_fd(), "PASS\r\n", strlen("PASS\r\n"), 0);
}

void	Command::nick(Command &command, User &user) {
	std::string msg = "NICK " + user.getNickName() + "\r\n";
	if (user.getNickName().empty() == false) {
		user.set_nickname(command.params[0]);
		send(user.get_fd(), msg.c_str(), msg.length(), 0);
	} else {
		user.set_nickname(command.params[0]);
	}
}

void	Command::user(Command &command, User &user) {
	std::string msg = "USER " + user.getUserName() + "\r\n";
	if (user.getUserName().empty() == false) {
		user.set_username(command.params[0]);
		send(user.get_fd(), msg.c_str(), msg.length(), 0);
	} else {
		user.set_username(command.params[0]);
	}
}

void	Command::motd(Command &command, User &user) {

}

void	Command::away(Command &command, User &user) {

}
