#include "../../incs/Server/NumericReplies.hpp"

void	sendMsg(int fd, std::string msg) {
	send(fd, msg.c_str(), msg.length(), 0);
}

std::string	RPL_YOURHOST() {
	return "002 rcollas :Your host is 127.0.0.1, running version 1.2.3\r\n";
}

std::string	RPL_CREATED() {
	return "003 rcollas :This server was created 18:07:30\r\n";
}

std::string RPL_MYINFO() {
	return "004 rcollas localhost 1.2.3 slave backroom [backroom hard]\r\n";
}

std::string	RPL_ISUPPORT() {
	return "005 rcollas 12 :are supported by this server\r\n";
}
