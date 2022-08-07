#include "../../incs/Server/NumericReplies.hpp"

std::string RPL_WELCOME() {
	return "001 rcollas :Welcome to the 127.0.0.1 Network, rcollas rcollas[!rcollas@127.0.0.1]\r\n";
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

/***************** ERRORS NUMERICS **************/

std::string	ERR_NEEDMOREPARAMS() {
	return "461 rcollas :Not enough parameters\r\n";
}

/***************** OTHER **************/

std::string	JOINWELCOMEMESSAGE(std::string channel) { // Changer le nom rcollas par le nom de user
	std::cout << "works" << std::endl;
	return ("rcollas is joining the channel #" + channel + "\r\n");
}
