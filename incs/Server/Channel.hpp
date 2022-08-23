#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Server.hpp"

#define checkUserInchannel chan->userInChannel(user.get_fd(), chan->getUsersList())
#define checkWaitingList chan->userInChannel(user.get_fd(), chan->getWaitingInviteList())
#define chanExist user.servInfo->channelExist(command.params[i]) 

class User;
struct Command;

class Channel {
public:
/***************** Canonical form **************/
	Channel(std::string const &channelName, std::string const &key);
	Channel(Channel const &src);
	~Channel();
	Channel &operator=(Channel const &obj);

/***************** getters functions **************/
	std::string 			getChannelName();
	std::string				getKeyName();
	std::string 			getTopic();
	std::map<int, User *>	getUsersList();
	bool 					getChannelJoined();
	bool					getInviteMode();	
	bool					getKeyExist();
	std::map<int, User *>	getWaitingInviteList();
	User					*getOperator();

/***************** members functions **************/
	bool		TopicIsSet();
	void		addUserToChannel(int fd, User *user);
	void		displayListChannelUsers();
	void		joinChannel(char *buffer);
	bool		userInChannel(int fd, std::map<int, User *> list);
	void		changeTopic(std::string topic);
	void		printChannelUsers(int fd, User *user, std::string channelName);
	void		removeUserChannel(int fd, User *user, std::string message);
	int			getNbUsers();
	void		inviteModeSetTrue();
	void		inviteModeSetFalse();
	void		setKey(std::string);
	void		setKeyExistTrue();
	void		setKeyExistFalse();
	void		addUserToWitingList(int fd, User *user);
	void		setOpUser(User *user);

	std::string 				_topic;

private :
	std::string 				_channelName;
	std::string 				_key;
	bool						_keyExist;
	std::map<int, User *> 		_usersList;
	std::map<int, User *> 		_waitingInviteList;
	bool						_inviteModeSet;
	//std::string 				_topic;
	bool 						_channelJoined;
	bool						_topicSet;
	User						*_opUser;
};
#endif