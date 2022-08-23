#include "../../incs/ft_irc.h"
#include "../../incs/Server/Server.hpp"

void	Command::sendPrivateMessage(User &user, std::string nickName, std::string message)
{
	User *nick = &user.servInfo->nickToUserFd(nickName);
	sendMsg(nick->get_fd(), PRV_MSG(user.getNickName(), message));
}

void	Command::sendMessageToChannel(User &user, std::string chanName, std::string message)
{
	Channel *channel= &user.servInfo->getChannel(chanName);
	std::map<int, User *> UsersList;
	UsersList = channel->getUsersList();
	std::map<int, User *>::iterator it;
	it = UsersList.begin();
	for (; it != UsersList.end(); it++)
		sendPrivateMessage(user, it->second->getNickName(), message);
}

void	Command::error(std::string msg, User &user) {
	sendMsg(user.get_fd(), msg);
	user.servInfo->killConnection(user);
}

void	Command::pass(Command &command, User &user) {
	if (command.params.size() == 1) {
		if (command.params.size() == 1 &&
			command.params[0] != user.servInfo->getPassword())
		{
			sendMsg(user.get_fd(), ERR_PASSWDMISMATCH(user.getNickName()));
			error(RED + "Fatal error: invalid connection password" + EOC, user);
		} else {
			user.setPasswdStatus(true);
		}
	} else if (command.params.size() < 1) {
		error(ERR_NEEDMOREPARAMS(user.getNickName()), user);
	}
}

void	Command::nick(Command &command, User &user) {
	if (command.params.size() > 1)
		return ;
	else if (emptyCommand == true)
		sendMsg(user.get_fd(), ERR_NONICKNAMEGIVEN());
	else {
		if (user.getNickName() == command.params[0] || user.servInfo->nicknameExists(command.params[0]) == true)
			sendMsg(user.get_fd(), ERR_NICKNAMEINUSE(user.getNickName()));
		else if (command.params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_-") != std::string::npos)
			sendMsg(user.get_fd(), ERR_ERRONEUSNICKNAME(user.getNickName()));
		else {
			if (user.getNickName().empty() == true) {
				user.set_nickname(command.params[0]);
				std::cout << "Nickname is set at : " << command.params[0] << std::endl;
				return ;
			}
			else {
				user.servInfo->sendToAll(user.get_fd(), "\033[0;31m" + user.getNickName() + "!@localhost NICK " + command.params[0] + "\r\n\033[0m");
				user.set_nickname(command.params[0]);
				std::cout << "Nickname is set at " << command.params[0] << std::endl;
			}
		}
	}
}

void	Command::user(Command &command, User &user) {
	if (command.params.size() > 4)
		return ;
	if (command.params.size() < 4) {
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
		return ;
	}
	if (!(command.params[1] == "0" && command.params[2] == "*")) {
		std::cout << "Usage: /USER <username> 0 * <realname>\n";
		return ;
	}
	if (command.params[0].length() < 1 || command.params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
		std::cout << "Username contains invalid characters\n";
	}
	if (command.params[3].length() < 1 || command.params[3].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
		std::cout << "Realname contains invalid characters\n";
		return ;
	}
	if (user.getUserName().empty() == false) {
		sendMsg(user.get_fd(), ERR_ALREADYREGISTERED(user.getNickName()));
		return ;
	}
	if (user.servInfo->usernameExists(command.params[0]) == true)
		std::cout << "Username already exists on the server\n";
	else {
		user.set_username(command.params[0]);
		std::cout << "Username is set at : " << command.params[0] << std::endl;
		user.set_realname(command.params[3]);
		std::cout << "Realname is set at : " << command.params[3] << std::endl;
		Server::welcome(user);
	}
}

void	Command::privmsg(Command &command, User &user) {
	if (command.params.size() != 2) {
		return ;
	}
	if (user.getNickName() == command.params[0])
		return ;
	if (user.servInfo->usernameExists(command.params[0]) == true || user.servInfo->nicknameExists(command.params[0]) == true) {
		if (user.servInfo->getAwayStatus(command.params[0]) == true) {
			sendMsg(user.get_fd(), "\033[0;31m301 " + command.params[0] + " :" + user.servInfo->getAwayString(command.params[0]) + "\r\n\033[0m");
		}
		sendMsg(user.servInfo->getTargetFd(command.params[0]), "\033[1;32m" + command.params[1] + "\r\n\033[0m");
	}
	if (user.servInfo->channelExist(command.params[0]) == true)
	{
		std::string ChanName = command.params[0];
		std::string message;
			message = command.params[1];
		sendMessageToChannel(user, ChanName, message);
	}
}

void	Command::motd(Command &command, User &user) {
	if (emptyCommand == true) {
		sendMsg(user.get_fd(), RPL_MOTDSTART(user.getNickName(), "localhost"));
		sendMsg(user.get_fd(), RPL_MOTD(user.getNickName(), "Welcooooooooooome at home!"));
		sendMsg(user.get_fd(), RPL_ENDOFMOTD(user.getNickName()));
	}
}

bool 	modeInviteChannel(Command &command, User &user)
{
	if (isAllowedMode(command.params[1]) == true && user.servInfo->channelExist(command.params[0]) == true
		&& command.params[1] == "+i" && command.params.size() == 2)
	{
		Channel *chan = &user.servInfo->getChannel(command.params[0]);
		chan->inviteModeSetTrue();
		int modesNumber = user.getModesNumber();
		user.set_modesNumber(++modesNumber);
		return (true);
	}
	return (false);
}

bool	modeKeyChannel(Command &command, User &user)
{
	if (isAllowedMode(command.params[1]) && user.servInfo->channelExist(command.params[0]) == true
	 	&& command.params[1] == "+k")
	{
		if (command.params.size() != 3)
			sendMsg(user.get_fd(), ERR_INVALIDKEY(user.getNickName(), command.params[0]));
		Channel *chan = &user.servInfo->getChannel(command.params[0]);
		chan->setKeyExistTrue();
		chan->setKey(command.params[2]);
		int modesNumber = user.getModesNumber();
		user.set_modesNumber(++modesNumber);
		return (true);
	}
	return false;
}

bool	modeErrorsCheck(Command &command, User &user)
{
	if (user.servInfo->nicknameExists(command.params[0]) == false) {
		sendMsg(user.get_fd(), ERR_NOSUCHNICK(user.getNickName(), command.params[0]));
		return true;
	}
	if (command.params[0] != user.getNickName()) {
		sendMsg(user.get_fd(), ERR_USERSDONTMATCH(user.getNickName()));
		return true;
	}
	if (command.params[0] == user.getNickName() && command.params.size() == 1) { // display user modes if no 2nd param
		char *str = ft_itoa(user.servInfo->getModes(user.getNickName()));
		sendMsg(user.get_fd(), RPL_UMODEIS(user.getNickName(), str));
		free(str);
		return true;
	}
	if (!isAllowedMode(command.params[1])) {
		sendMsg(user.get_fd(), ERR_UMODEUNKNOWNFLAG(user.getNickName()));
		return true;
	}
	return false;
}

void	Command::oper(Command &command, User &user) {
	if (command.params.empty() == true || command.params.size() != 2) {
		sendMsg(user.get_fd(), ERR_NEEDMOREPARAMS(user.getNickName()));
		return ;
	}
	if (command.params[0] != user.getUserName()) {
		return ;
	}
	if (command.params[1] != user.servInfo->getPassword()) {
		sendMsg(user.get_fd(), ERR_PASSWDMISMATCH(user.getNickName()));
		return ;
	}
	if (user.getIsOperator() == false) {
		user.set_isOperator(true);
		int modesNumber = user.getModesNumber();
		user.set_modesNumber(++modesNumber);
		int nbOfOperators = user.servInfo->getNbOfOperators();
		user.servInfo->set_nbOfOperators(++nbOfOperators);
		sendMsg(user.get_fd(), RPL_YOUREOPER(user.getNickName()));
	}
}

void	Command::mode(Command &command, User &user) {
	if (command.params.size() > 4 || command.params.empty() == true)
		return ;
	if (modeInviteChannel(command, user) == true)
		return;
	else if (modeKeyChannel(command, user) == true)
		return;
	else if (modeErrorsCheck(command, user) == true)
		return;
}

void	Command::away(Command &command, User &user) {
	if (emptyCommand == true && user.getIsAway() == true) {
		user.set_isAway(false);
		sendMsg(user.get_fd(), RPL_UNAWAY(user.getNickName()));
	}
	if (emptyCommand == false) {
		user.set_isAway(true);
		user.set_awayMessage(command.params[0]);
		sendMsg(user.get_fd(), RPL_NOWAWAY(user.getNickName()));
		sendMsg(user.get_fd(), RPL_AWAY(user.getNickName(), command.params[0]));
	}
}

void	Command::version(Command &command, User &user) {
	if (emptyCommand == true)
		sendMsg(user.get_fd(), RPL_VERSION(user.getNickName()));
}

void	Command::lusers(Command &command, User &user) {

	if (emptyCommand == true) {

		char *str = ft_itoa(user.servInfo->getNbOfUsers());
		char *arr = ft_itoa(user.servInfo->getNbOfOperators());
		sendMsg(user.get_fd(), RPL_LUSERCLIENT(user.getNickName(), str));
		sendMsg(user.get_fd(), RPL_LUSEROP(user.getNickName(), arr));
		sendMsg(user.get_fd(), RPL_LUSERCHANNELS(user.getNickName(), "getNumberOfChan")); // A REMPLIR AVEC FONCTION GETNUMBEROFCHAN
		free(str);
		free(arr);
	}
}
