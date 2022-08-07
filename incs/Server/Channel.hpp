#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Server.hpp"


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
	void		createChannel(); //infos à envoyer à Robin
	void		addUserToChannel();
	void		displayListChannelUsers();
	void		joinChannel(char *buffer);

private :
	std::string _channelName;
	std::string _key;
	std::vector<std::string> _usersList; // to change by client send by Robin
	std::string _topic;

};
#endif