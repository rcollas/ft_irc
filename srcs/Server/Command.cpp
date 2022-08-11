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
	if (command.params[0].empty()) {
		sendMsg(user.get_fd(), RPL_MOTDSTART(user.getNickName(), "localhost"));
		sendMsg(user.get_fd(), RPL_MOTD(user.getNickName(), "Welcooooooooooome at home!"));
		sendMsg(user.get_fd(), RPL_ENDOFMOTD(user.getNickName()));
	}
}

void	Command::away(Command &command, User &user) {
	if (command.params[0].empty()) {
		user.set_isAway(false);
		sendMsg(user.get_fd(), RPL_UNAWAY(user.getNickName()));
	}
	if (command.params[0].length()){
		user.set_isAway(true);
		sendMsg(user.get_fd(), RPL_NOWAWAY(user.getNickName()));
		sendMsg(user.get_fd(), RPL_AWAY(user.getNickName(), command.params[0]));
	}
}

void	Command::version(Command &command, User &user){
	if (command.params[0].empty()) {
		sendMsg(user.get_fd(), RPL_VERSION(user.getNickName()));
	}
}

void	Command::lusers(Command &command, User &user){
	if (command.params[0].empty()) {
		sendMsg(user.get_fd(), RPL_LUSERCLIENT(user.getNickName()));
		sendMsg(user.get_fd(), RPL_LUSEROP(user.getNickName()));
		sendMsg(user.get_fd(), RPL_LUSERCHANNEL(user.getNickName()));
		sendMsg(user.get_fd(), RPL_LUSERME(user.getNickName()));
	}
}
/***************** CHANNEL COMMAND **************/

void	Command::join(Command &command, User &user) {
	std::cout << "====>><< INSIDE" << std::endl;
	if (command.params[0].length())
	{
		Channel *vincent = new Channel(command.params[0], "0");
		std::cout << "PROUT" << std::endl;
		(void) vincent;
	}
	else
	{
		send(user.get_fd(), ERR_NEEDMOREPARAMS().c_str(),
			strlen(ERR_NEEDMOREPARAMS().c_str()), 0);
		return;
	}
	// if (user.getUserName().empty() == false) {
	// 	user.set_username(command.params[0]);
	// 	send(user.get_fd(), msg.c_str(), msg.length(), 0);
	// } else {
	// 	user.set_username(command.params[0]);
	// }
}
