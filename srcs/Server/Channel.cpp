#include "../../incs/Server/Channel.hpp"

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
