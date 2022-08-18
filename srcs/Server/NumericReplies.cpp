#include "../../incs/Server/NumericReplies.hpp"

void	sendMsg(int fd, std::string msg) {
	send(fd, msg.c_str(), msg.length(), 0);
}