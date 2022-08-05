#ifndef FT_IRC_USER_HPP
#define FT_IRC_USER_HPP

#include <iostream>

/*
 * Check if registration is complete
 * Send a PING and ask for a PONG
 * if registration is complete server sends RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO and RPL_ISUPPORT
 * ex: std::string RPL_WELCOME(std::string prefix) { return "Welcome to the server" + prefix; }
 * everytime a client use a cmd, the server must send specific message
 * ex: JOIN cmd, a join message, RPL_TOPIC, RPL_NAMEPLY...
 */

class User {

	private:
		std::string	nick_name;
		std::string	user_name;
		std::string	real_name;

	public:
		User() {};
		~User() {};
};


#endif
