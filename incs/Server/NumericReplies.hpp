#ifndef FT_IRC_NUMERICREPLIES_HPP
#define FT_IRC_NUMERICREPLIES_HPP

#include <iostream>

std::string	RPL_WELCOME();
std::string RPL_YOURHOST();
std::string	RPL_CREATED();
std::string	RPL_MYINFO();
std::string RPL_ISUPPORT();
std::string RPL_ISUPPORT();

/***************** NUMERICS ERRORS **************/

std::string ERR_NEEDMOREPARAMS();

/***************** OTHER **************/
std::string	JOINWELCOMEMESSAGE(std::string channel);

#endif
