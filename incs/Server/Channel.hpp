#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Server.hpp"

class User;
struct Command;

class Channel {
public:
/***************** Canonical form **************/
	Channel(std::string const &channelName, std::string const &key);
	Channel(Channel const &src);
	~Channel();
	Channel &operator=(Channel const &obj);

/***************** members functions **************/
	std::string getChannelName();
	std::string getKeyName();
	std::string getTopic();
	bool 		getChannelJoined();
	void		addUserToChannel(int fd, User *user);
	void		displayListChannelUsers();
	void		joinChannel(char *buffer);
	bool		userInChannel(int fd);
	void		changeTopic(std::string topic);
	void		printChannelUsers(int fd, User *user);

	std::string 				_topic;

private :
	std::string 				_channelName;
	std::string 				_key;
	std::map<int, User *> 		_usersList; 
	//std::string 				_topic;
	bool 						_channelJoined;
};
#endif