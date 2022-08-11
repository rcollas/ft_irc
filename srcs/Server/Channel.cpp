#include "../../incs/Server/Channel.hpp"
#include "../../incs/Server/Server.hpp"


/***************** Canonical form **************/

Channel::Channel(std::string const &channelName, std::string const &key)
{
	this->_channelName 		= channelName;
	this->_key				= key;
	this->_channelJoined	= true;
	this->_topic			= "NOT WORKING";
	std::cout << "Constructor is called" << std::endl;
}

Channel::Channel(Channel const &src)
{
	std::cout << "Constructor with grade is called" << std::endl;
	*this = src; // copy all the element of src in the current instance
}

Channel::~Channel()
{
	std::cout << "Destructor is called" << std::endl;
}

Channel &Channel::operator=(Channel const &obj)
{
	this->_channelName = obj._channelName;
	this->_key = obj._key;
	this->_topic = obj._topic;
	this->_usersList = obj._usersList;
	std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/***************** Get members functions **************/

std::string	Channel::getChannelName()
{
	return (this->_channelName);
}

std::string Channel::getKeyName()
{
	return (this->_key);
}

std::string Channel::getTopic()
{
	return (this->_topic);
}

bool 		Channel::getChannelJoined()
{
	return (this->_channelJoined);
}
/***************** Members functions **************/
void		Channel::channelIsJoined()
{
	this->_channelJoined = true;
}

void		Channel::changeTopic(std::string topic)
{
	this->_topic = topic;
}

void		Channel::addUserToChannel(int fd, User *user)
{
	this->_usersList.insert(std::pair<int, User *>(fd, user));
}

void		Channel::printChannelUsers(int fd, User *user)
{
	std::map<int, User *>::iterator it; // On donne le type à l'itérator
	it = this->_usersList.begin(); // On le met au début
	for(; it != this->_usersList.end(); it++)
	{
		std::cout <<  "\033[0;31m" << "LE USER NAME EST " << it->second->getUserName() << "\033[0m"  << std::endl; // second sert à chercher la value de l'itérateur
	}
	(void) fd;
	(void) user;
}
