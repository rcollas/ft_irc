#include "../../incs/Server/Channel.hpp"
#include "../../incs/Server/Server.hpp"


/*
**==========================
**   CANONICAL FORM
**==========================
*/

Channel::Channel(std::string const &channelName, std::string const &key)
{
	this->_channelName 		= channelName;
	this->_key				= key;
	this->_channelJoined	= true;
	this->_topicSet			= false;
	this->_inviteModeSet	= false;
	this->_keyExist			= false;
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

/*
**==========================
**  GET MEMBER FUNCTIONS
**==========================
*/

std::string				Channel::getChannelName()
{
	return (this->_channelName);
}

std::string 			Channel::getKeyName()
{
	return (this->_key);
}

std::string 			Channel::getTopic()
{
	return (this->_topic);
}

bool 					Channel::getChannelJoined()
{
	return (this->_channelJoined);
}

std::map<int, User *>	Channel::getUsersList()
{
	return this->_usersList;
}

bool					Channel::getInviteMode()
{
	return (this->_inviteModeSet);
}

bool					Channel::getKeyExist()
{
	return (this->_keyExist);
}

std::map<int, User *>	Channel::getWaitingInviteList()
{
	return this->_waitingInviteList;
}


/*
**==========================
**    MEMBER FUNCTIONS
**==========================
*/

/***************** 
 * Here we check if the user is in the channel 
 * by using map who call the class User
 *  **************/
bool		Channel::userInChannel(int fd, std::map<int, User *> list)
{
	if (list.find(fd) == list.end())
		return (false);
	return (true);
}

void		Channel::changeTopic(std::string topic)
{
	this->_topicSet = true;
	this->_topic = topic;
}

/***************** Here I add a user to the channel  **************/
void		Channel::addUserToChannel(int fd, User *user)
{
	this->_usersList.insert(std::pair<int, User *>(fd, user));
}

void		Channel::addUserToWitingList(int fd, User *user)
{
	this->_waitingInviteList.insert(std::pair<int, User *>(fd, user));
}

/***************** Here I print all the user of the channel **************/
void		Channel::printChannelUsers(int fd, User *user, std::string channelName)
{
	std::map<int, User *>::iterator it; // On donne le type à l'itérator
	it = this->_usersList.begin(); // On le met au début
	for(; it != this->_usersList.end(); it++)
		sendMsg(fd, RPL_NAMREPLY(it->second->getNickName(), channelName));
	sendMsg(fd, RPL_ENDOFNAMES(channelName));
	(void) user;
}

void		Channel::removeUserChannel(int fd, User *user)
{
	if (this->userInChannel(fd, this->getUsersList()) == true)
	{
		this->_usersList.erase(fd);
		sendMsg(fd, PART_LEAVE_CHANNEL_MESSAGE(user->getNickName(), this->getChannelName()));
	}
	else 
		std::cout <<  "\033[0;31m" << "I am not this channel" << "\033[0m" << std::endl;

}

int		Channel::getNbUsers(void)
{
	return (this->_usersList.size());
}

void	Channel::inviteModeSetTrue()
{
	this->_inviteModeSet = true;
}

void	Channel::inviteModeSetFalse()
{
	this->_inviteModeSet = false;
}
void	Channel::setKeyExistTrue()
{
	this->_keyExist = true;
}

void	Channel::setKeyExistFalse()
{
	this->_keyExist = false;
}

bool	Channel::TopicIsSet()
{
	return (this->_topicSet);
}

void	Channel::setKey(std::string key)
{
	this->_key = key;
}	